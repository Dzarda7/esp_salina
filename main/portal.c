/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Setup portal.
 *
 * An unprovisioned board raises its own access point and serves a two step
 * form: pick a network, then name a stop. The stop step needs the real API, so
 * the board runs as AP and station at once - your phone stays on the setup
 * network while the board talks to the internet behind it. The radio has one
 * channel, so the AP follows the station onto the router's channel when it
 * joins, which drops connected phones for a moment; the page polls through it.
 */
#include <string.h>

#include "cJSON.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portal_dns.h"
#include "salina.h"
#include "salina_config.h"

static const char *TAG = "portal";

#define AP_CHANNEL      1
#define AP_MAX_CLIENTS  2
#define PORTAL_URL      SALINA_AP_URL "/"
#define JOIN_TIMEOUT_MS 20000
#define SCAN_MAX        20
#define BODY_MAX        512
/* The stop search does a TLS request from inside the request handler, and
 * mbedTLS with the certificate bundle needs far more stack than the default. */
#define HTTPD_STACK     16384

typedef enum { STATE_IDLE, STATE_CONNECTING, STATE_CONNECTED, STATE_FAILED } join_state_t;

extern const char portal_html_start[] asm("_binary_portal_html_start");
extern const char portal_html_end[] asm("_binary_portal_html_end");

static httpd_handle_t s_httpd;
static esp_netif_t *s_ap_netif;
static esp_netif_t *s_sta_netif;
static salina_config_t s_pending;
static volatile join_state_t s_state;
static volatile bool s_saved;
static int64_t s_join_deadline;
static wifi_ap_record_t s_scan[SCAN_MAX];
static uint16_t s_scan_count;

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "joined %s", s_pending.ssid);
        s_state = STATE_CONNECTED;
    } else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_state == STATE_CONNECTING && esp_timer_get_time() < s_join_deadline) {
            esp_wifi_connect(); /* a wrong password also lands here, so we time out */
        } else if (s_state == STATE_CONNECTING) {
            s_state = STATE_FAILED;
        }
    }
}

/* --- handlers --- */

static esp_err_t root_get(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, portal_html_start, portal_html_end - portal_html_start - 1);
}

static esp_err_t send_json(httpd_req_t *req, cJSON *json)
{
    char *text = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    ESP_RETURN_ON_FALSE(text, ESP_ERR_NO_MEM, TAG, "no mem for json");

    httpd_resp_set_type(req, "application/json");
    const esp_err_t ret = httpd_resp_sendstr(req, text);
    free(text);
    return ret;
}

/**
 * @brief Take the survey of nearby networks.
 *
 * Done once, before the web server accepts anyone: a scan makes the radio hop
 * channels, which stalls the access point for a second or two. With a phone
 * already attached that shows up as the setup page failing to load.
 */
static void scan_networks(void)
{
    if (esp_wifi_scan_start(NULL, true) != ESP_OK) {
        ESP_LOGW(TAG, "scan failed, the network has to be typed in");
        s_scan_count = 0;
        return;
    }
    s_scan_count = SCAN_MAX;
    esp_wifi_scan_get_ap_records(&s_scan_count, s_scan);
    ESP_LOGI(TAG, "%u networks nearby", s_scan_count);
}

static esp_err_t scan_get(httpd_req_t *req)
{
    const uint16_t found = s_scan_count;
    const wifi_ap_record_t *records = s_scan;

    cJSON *list = cJSON_CreateArray();
    for (int i = 0; i < found; i++) {
        const char *ssid = (const char *)records[i].ssid;
        if (!ssid[0]) {
            continue; /* hidden network, nothing to show */
        }
        bool duplicate = false;
        for (int j = 0; j < i && !duplicate; j++) {
            duplicate = strcmp(ssid, (const char *)records[j].ssid) == 0;
        }
        if (duplicate) {
            continue;
        }
        cJSON *entry = cJSON_CreateObject();
        cJSON_AddStringToObject(entry, "ssid", ssid);
        cJSON_AddNumberToObject(entry, "rssi", records[i].rssi);
        cJSON_AddItemToArray(list, entry);
    }
    return send_json(req, list);
}

/** @brief Read a JSON request body into a parsed document. */
static cJSON *read_body(httpd_req_t *req)
{
    char body[BODY_MAX];

    if (req->content_len >= sizeof(body)) {
        return NULL;
    }
    int received = 0;
    while (received < req->content_len) {
        const int n = httpd_req_recv(req, body + received, req->content_len - received);
        if (n <= 0) {
            return NULL;
        }
        received += n;
    }
    body[received] = '\0';
    return cJSON_Parse(body);
}

static esp_err_t connect_post(httpd_req_t *req)
{
    cJSON *body = read_body(req);
    ESP_RETURN_ON_FALSE(body, httpd_resp_send_500(req), TAG, "bad body");

    const cJSON *ssid = cJSON_GetObjectItemCaseSensitive(body, "ssid");
    const cJSON *pass = cJSON_GetObjectItemCaseSensitive(body, "pass");
    if (!cJSON_IsString(ssid)) {
        cJSON_Delete(body);
        return httpd_resp_send_500(req);
    }
    strlcpy(s_pending.ssid, ssid->valuestring, sizeof(s_pending.ssid));
    strlcpy(s_pending.password, cJSON_IsString(pass) ? pass->valuestring : "", sizeof(s_pending.password));
    cJSON_Delete(body);

    wifi_config_t sta = { 0 };
    strlcpy((char *)sta.sta.ssid, s_pending.ssid, sizeof(sta.sta.ssid));
    strlcpy((char *)sta.sta.password, s_pending.password, sizeof(sta.sta.password));
    esp_wifi_set_config(WIFI_IF_STA, &sta);

    s_state = STATE_CONNECTING;
    s_join_deadline = esp_timer_get_time() + (int64_t)JOIN_TIMEOUT_MS * 1000;
    esp_wifi_connect();
    return httpd_resp_sendstr(req, "{}");
}

static esp_err_t status_get(httpd_req_t *req)
{
    static const char *names[] = { "idle", "connecting", "connected", "failed" };

    if (s_state == STATE_CONNECTING && esp_timer_get_time() > s_join_deadline) {
        s_state = STATE_FAILED;
    }
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "state", names[s_state]);
    return send_json(req, json);
}

/** @brief Turn %xx escapes and + back into bytes, in place. */
static void url_decode(char *text)
{
    char *out = text;

    for (const char *in = text; *in; in++) {
        if (*in == '%' && in[1] && in[2]) {
            char hex[3] = { in[1], in[2], '\0' };
            *out++ = (char)strtol(hex, NULL, 16);
            in += 2;
        } else {
            *out++ = (*in == '+') ? ' ' : *in;
        }
    }
    *out = '\0';
}

static esp_err_t search_get(httpd_req_t *req)
{
    char query[128];
    char value[SALINA_STOP_LEN];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK ||
        httpd_query_key_value(query, "q", value, sizeof(value)) != ESP_OK) {
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "no query");
    }
    url_decode(value);

    ESP_LOGI(TAG,
             "searching \"%s\", %u bytes free, largest block %u",
             value,
             (unsigned)esp_get_free_heap_size(),
             (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

    salina_stop_info_t stop;
    const esp_err_t err = salina_lookup_stop(value, &stop);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "lookup failed: %s", esp_err_to_name(err));
        return httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, esp_err_to_name(err));
    }

    cJSON *json = cJSON_CreateObject();
    /* "name" is what gets stored and drawn; "full" always names the town, so
     * setup can show which stop a loose query landed on. */
    cJSON_AddStringToObject(json, "name", stop.name);
    cJSON_AddStringToObject(json, "full", stop.full_name);
    cJSON *list = cJSON_AddArrayToObject(json, "signs");
    for (int i = 0; i < stop.sign_count; i++) {
        cJSON *entry = cJSON_CreateObject();
        cJSON_AddNumberToObject(entry, "number", stop.signs[i].number);
        cJSON_AddStringToObject(entry, "description", stop.signs[i].description);
        cJSON_AddItemToArray(list, entry);
    }
    return send_json(req, json);
}

static esp_err_t save_post(httpd_req_t *req)
{
    cJSON *body = read_body(req);
    ESP_RETURN_ON_FALSE(body, httpd_resp_send_500(req), TAG, "bad body");

    const cJSON *stop = cJSON_GetObjectItemCaseSensitive(body, "stop");
    const cJSON *left = cJSON_GetObjectItemCaseSensitive(body, "left");
    const cJSON *right = cJSON_GetObjectItemCaseSensitive(body, "right");
    if (!cJSON_IsString(stop) || !cJSON_IsNumber(left) || !cJSON_IsNumber(right)) {
        cJSON_Delete(body);
        return httpd_resp_send_500(req);
    }
    strlcpy(s_pending.stop, stop->valuestring, sizeof(s_pending.stop));
    s_pending.sign_left = left->valueint;
    s_pending.sign_right = right->valueint;
    cJSON_Delete(body);

    const esp_err_t ret = salina_config_save(&s_pending);
    if (ret != ESP_OK) {
        return httpd_resp_send_500(req);
    }
    httpd_resp_sendstr(req, "{}");
    s_saved = true;
    return ESP_OK;
}

/** @brief Anything else is a phone probing for a captive portal: point it home. */
static esp_err_t redirect_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", PORTAL_URL);
    return httpd_resp_send(req, NULL, 0);
}

/* --- lifecycle --- */

static esp_err_t start_ap(void)
{
    ESP_RETURN_ON_ERROR(esp_netif_init(), TAG, "netif init");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), TAG, "event loop");
    s_ap_netif = esp_netif_create_default_wifi_ap();
    s_sta_netif = esp_netif_create_default_wifi_sta();
    ESP_RETURN_ON_FALSE(s_ap_netif && s_sta_netif, ESP_FAIL, TAG, "netif create");

    const wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_wifi_init(&init_cfg), TAG, "wifi init");
    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            wifi_event_handler,
                                                            NULL,
                                                            NULL),
                        TAG,
                        "wifi events");
    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            wifi_event_handler,
                                                            NULL,
                                                            NULL),
                        TAG,
                        "ip events");

    /* Open, so there is no password to read off the glass and mistype. It is
     * only up while you are setting the board up, and it is next to you. */
    wifi_config_t ap = {
        .ap = {
            .channel = AP_CHANNEL,
            .max_connection = AP_MAX_CLIENTS,
            .authmode = WIFI_AUTH_OPEN,
        },
    };
    strlcpy((char *)ap.ap.ssid, SALINA_AP_SSID, sizeof(ap.ap.ssid));
    ap.ap.ssid_len = strlen(SALINA_AP_SSID);

    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_APSTA), TAG, "wifi mode");
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_AP, &ap), TAG, "ap config");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "wifi start");
    /* Same reason the station drops to 13 dBm: a full power radio browns this
     * board out. An access point you stand next to needs nothing more. */
    ESP_RETURN_ON_ERROR(esp_wifi_set_max_tx_power(52), TAG, "tx power");

    /* Tell modern phones where the portal lives, so they offer it rather than
     * deciding the network is broken. The lease server has to be still for it. */
    const char *uri = PORTAL_URL;
    esp_netif_dhcps_stop(s_ap_netif);
    esp_netif_dhcps_option(s_ap_netif, ESP_NETIF_OP_SET, ESP_NETIF_CAPTIVEPORTAL_URI, (void *)uri, strlen(uri));
    esp_netif_dhcps_start(s_ap_netif);
    return ESP_OK;
}

static esp_err_t start_server(void)
{
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.stack_size = HTTPD_STACK;
    cfg.max_uri_handlers = 8;
    cfg.lru_purge_enable = true;
    ESP_RETURN_ON_ERROR(httpd_start(&s_httpd, &cfg), TAG, "httpd");

    static const httpd_uri_t routes[] = {
        { .uri = "/", .method = HTTP_GET, .handler = root_get },
        { .uri = "/scan", .method = HTTP_GET, .handler = scan_get },
        { .uri = "/status", .method = HTTP_GET, .handler = status_get },
        { .uri = "/search", .method = HTTP_GET, .handler = search_get },
        { .uri = "/connect", .method = HTTP_POST, .handler = connect_post },
        { .uri = "/save", .method = HTTP_POST, .handler = save_post },
    };
    for (size_t i = 0; i < sizeof(routes) / sizeof(routes[0]); i++) {
        ESP_RETURN_ON_ERROR(httpd_register_uri_handler(s_httpd, &routes[i]), TAG, "route");
    }
    return httpd_register_err_handler(s_httpd, HTTPD_404_NOT_FOUND, redirect_handler);
}

esp_err_t salina_portal_run(void)
{
    ESP_RETURN_ON_ERROR(start_ap(), TAG, "ap");
    scan_networks();
    ESP_RETURN_ON_ERROR(start_server(), TAG, "server");

    esp_netif_ip_info_t ip = { 0 };
    esp_netif_get_ip_info(s_ap_netif, &ip);
    portal_dns_start(ip.ip.addr);

    ESP_LOGI(TAG, "setup portal on open network \"%s\", %s", SALINA_AP_SSID, PORTAL_URL);
    ESP_LOGI(TAG,
             "%u bytes of heap free, largest block %u",
             (unsigned)esp_get_free_heap_size(),
             (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

    /* No deadline: a board that does not know its network has nothing useful to
     * do with the time it would save by sleeping, so it waits to be told. */
    while (!s_saved) {
        vTaskDelay(pdMS_TO_TICKS(250));
    }

    portal_dns_stop();
    httpd_stop(s_httpd);
    s_httpd = NULL;
    esp_wifi_stop();
    esp_wifi_deinit();
    return ESP_OK;
}
