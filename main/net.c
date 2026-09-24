/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Wi-Fi association and SNTP, both with a bounded wait.
 */
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "esp_check.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_sntp.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "salina.h"

static const char *TAG = "net";

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAILED_BIT    BIT1
#define WIFI_MAX_RETRY     5
#define WIFI_TIMEOUT_MS    20000
#define SNTP_TIMEOUT_MS    15000

/* Central European time, with the EU daylight saving rules. */
#define SALINA_TZ          "CET-1CEST,M3.5.0,M10.5.0/3"

static EventGroupHandle_t s_wifi_events;
static esp_netif_t *s_netif;
static int s_retries;

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retries++ < WIFI_MAX_RETRY) {
            esp_wifi_connect();
        } else {
            xEventGroupSetBits(s_wifi_events, WIFI_FAILED_BIT);
        }
    } else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        s_retries = 0;
        xEventGroupSetBits(s_wifi_events, WIFI_CONNECTED_BIT);
    }
}

/** @brief Already-initialised is success: these two are process-wide. */
static esp_err_t init_once(esp_err_t err)
{
    return (err == ESP_ERR_INVALID_STATE) ? ESP_OK : err;
}

esp_err_t salina_net_connect(const char *ssid, const char *password)
{
    esp_err_t ret = ESP_OK;

    s_wifi_events = xEventGroupCreate();
    ESP_RETURN_ON_FALSE(s_wifi_events, ESP_ERR_NO_MEM, TAG, "no mem");

    ESP_GOTO_ON_ERROR(init_once(esp_netif_init()), fail, TAG, "netif init");
    ESP_GOTO_ON_ERROR(init_once(esp_event_loop_create_default()), fail, TAG, "event loop");
    s_netif = esp_netif_create_default_wifi_sta();
    ESP_GOTO_ON_FALSE(s_netif, ESP_FAIL, fail, TAG, "netif create");

    const wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_GOTO_ON_ERROR(esp_wifi_init(&init_cfg), fail, TAG, "wifi init");
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL),
                      fail,
                      TAG,
                      "wifi events");
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_register(IP_EVENT,
                                                          IP_EVENT_STA_GOT_IP,
                                                          wifi_event_handler,
                                                          NULL,
                                                          NULL),
                      fail,
                      TAG,
                      "ip events");

    wifi_config_t wifi_cfg = {
        .sta = {
            /* WPA2 as the floor still associates with a WPA3 transition AP,
             * while refusing to fall back to an open one. */
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
        },
    };
    strlcpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid));
    strlcpy((char *)wifi_cfg.sta.password, password, sizeof(wifi_cfg.sta.password));

    ESP_GOTO_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), fail, TAG, "wifi mode");
    ESP_GOTO_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg), fail, TAG, "wifi config");
    /* Radio power is the dominant cost of a wake-up, so no power save games -
     * associate fast, do the work, switch it off again. */
    ESP_GOTO_ON_ERROR(esp_wifi_set_ps(WIFI_PS_NONE), fail, TAG, "wifi ps");
    ESP_GOTO_ON_ERROR(esp_wifi_start(), fail, TAG, "wifi start");
    /* Full transmit power browns out a board that is also driving an e-paper
     * panel off a thin USB supply; 13 dBm is plenty for a stationary display. */
    ESP_GOTO_ON_ERROR(esp_wifi_set_max_tx_power(52), fail, TAG, "tx power");

    const EventBits_t bits = xEventGroupWaitBits(s_wifi_events,
                                                 WIFI_CONNECTED_BIT | WIFI_FAILED_BIT,
                                                 pdFALSE,
                                                 pdFALSE,
                                                 pdMS_TO_TICKS(WIFI_TIMEOUT_MS));
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to %s", ssid);
        return ESP_OK;
    }
    ESP_LOGW(TAG, "could not join %s", ssid);
    ret = (bits & WIFI_FAILED_BIT) ? ESP_FAIL : ESP_ERR_TIMEOUT;

fail:
    /* Unwind whatever came up, so the caller can just sleep. */
    salina_net_disconnect();
    return ret;
}

void salina_net_disconnect(void)
{
    esp_wifi_stop();
    esp_wifi_deinit();
    if (s_netif) {
        esp_netif_destroy_default_wifi(s_netif);
        s_netif = NULL;
    }
    if (s_wifi_events) {
        vEventGroupDelete(s_wifi_events);
        s_wifi_events = NULL;
    }
}

bool salina_clock_is_set(void)
{
    return time(NULL) > 1700000000; /* somewhere in late 2023 */
}

void salina_time_set_tz(void)
{
    setenv("TZ", SALINA_TZ, 1);
    tzset();
}

esp_err_t salina_time_sync(void)
{
    /* Three servers rather than one: a single lost packet against a single
     * server does not retry inside our wait, which shows up as an occasional
     * unsynced clock. The Czech pool is first because it answers fastest. */
    esp_sntp_config_t cfg = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(3,
                                                                   ESP_SNTP_SERVER_LIST("cz.pool.ntp.org",
                                                                                        "pool.ntp.org",
                                                                                        "time.google.com"));

    ESP_RETURN_ON_ERROR(esp_netif_sntp_init(&cfg), TAG, "sntp init");
    const esp_err_t err = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(SNTP_TIMEOUT_MS));
    esp_netif_sntp_deinit();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "clock not synced: %s", esp_err_to_name(err));
        return err;
    }
    salina_time_set_tz();
    return ESP_OK;
}
