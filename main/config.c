/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Provisioned settings in NVS.
 *
 * Wi-Fi credentials used to be compiled in, which meant every binary carried
 * them and changing networks meant a rebuild. They now live in NVS, written by
 * the setup portal. An empty namespace is what sends the board to that portal.
 */
#include <string.h>

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "salina_config.h"

static const char *TAG = "config";

#define NVS_NAMESPACE "salina"
#define KEY_SSID      "ssid"
#define KEY_PASS      "pass"
#define KEY_STOP      "stop"
#define KEY_SIGN_L    "sign_l"
#define KEY_SIGN_R    "sign_r"

esp_err_t salina_config_load(salina_config_t *out)
{
    ESP_RETURN_ON_FALSE(out, ESP_ERR_INVALID_ARG, TAG, "null output");

    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs), TAG, "nvs open");

    esp_err_t ret = ESP_OK;
    /* A missing key just means "never provisioned", which is the normal state
     * of a fresh board, so it is not logged as a failure. */
    size_t len = sizeof(out->ssid);
    ret = nvs_get_str(nvs, KEY_SSID, out->ssid, &len);
    if (ret != ESP_OK) {
        goto out;
    }

    len = sizeof(out->password);
    if (nvs_get_str(nvs, KEY_PASS, out->password, &len) != ESP_OK) {
        out->password[0] = '\0'; /* an open network is legitimate */
    }
    len = sizeof(out->stop);
    ret = nvs_get_str(nvs, KEY_STOP, out->stop, &len);
    if (ret != ESP_OK) {
        goto out;
    }

    int32_t left = 0;
    int32_t right = 0;
    ret = nvs_get_i32(nvs, KEY_SIGN_L, &left);
    if (ret == ESP_OK) {
        ret = nvs_get_i32(nvs, KEY_SIGN_R, &right);
    }
    if (ret != ESP_OK) {
        goto out;
    }
    out->sign_left = left;
    out->sign_right = right;

out:
    nvs_close(nvs);
    return ret;
}

esp_err_t salina_config_save(const salina_config_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null config");

    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs), TAG, "nvs open");

    esp_err_t ret = ESP_OK;
    ESP_GOTO_ON_ERROR(nvs_set_str(nvs, KEY_SSID, cfg->ssid), out, TAG, "write ssid");
    ESP_GOTO_ON_ERROR(nvs_set_str(nvs, KEY_PASS, cfg->password), out, TAG, "write pass");
    ESP_GOTO_ON_ERROR(nvs_set_str(nvs, KEY_STOP, cfg->stop), out, TAG, "write stop");
    ESP_GOTO_ON_ERROR(nvs_set_i32(nvs, KEY_SIGN_L, cfg->sign_left), out, TAG, "write left");
    ESP_GOTO_ON_ERROR(nvs_set_i32(nvs, KEY_SIGN_R, cfg->sign_right), out, TAG, "write right");
    ESP_GOTO_ON_ERROR(nvs_commit(nvs), out, TAG, "commit");
    ESP_LOGI(TAG, "saved \"%s\", signs %d and %d", cfg->stop, cfg->sign_left, cfg->sign_right);

out:
    nvs_close(nvs);
    return ret;
}

esp_err_t salina_config_erase(void)
{
    nvs_handle_t nvs;
    esp_err_t ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs);
    if (ret != ESP_OK) {
        return ret;
    }
    nvs_erase_key(nvs, KEY_SSID);
    nvs_erase_key(nvs, KEY_PASS);
    nvs_erase_key(nvs, KEY_STOP);
    nvs_erase_key(nvs, KEY_SIGN_L);
    nvs_erase_key(nvs, KEY_SIGN_R);
    ret = nvs_commit(nvs);
    nvs_close(nvs);
    return ret;
}

bool salina_config_pin_asserted(void)
{
#if CONFIG_SALINA_CONFIG_PIN >= 0
    const gpio_config_t cfg = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pin_bit_mask = 1ULL << CONFIG_SALINA_CONFIG_PIN,
    };
    if (gpio_config(&cfg) != ESP_OK) {
        return false;
    }
    return gpio_get_level(CONFIG_SALINA_CONFIG_PIN) == 0;
#else
    return false;
#endif
}
