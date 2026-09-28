/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Shared HTTPS fetch.
 *
 * The whole reply is buffered before parsing, which is fine because every
 * response either source asks for is a few kilobytes. Compression is
 * deliberately not requested: nothing here can inflate it.
 */
#include <ctype.h>
#include <string.h>

#include "esp_check.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "salina_http.h"

static const char *TAG = "http";

typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} response_t;

static esp_err_t http_event(esp_http_client_event_t *evt)
{
    response_t *resp = (response_t *)evt->user_data;

    if (evt->event_id != HTTP_EVENT_ON_DATA || !resp) {
        return ESP_OK;
    }
    if (resp->len + evt->data_len >= resp->cap) {
        ESP_LOGE(TAG, "response larger than %u bytes", (unsigned)resp->cap);
        return ESP_FAIL;
    }
    memcpy(resp->buf + resp->len, evt->data, evt->data_len);
    resp->len += evt->data_len;
    resp->buf[resp->len] = '\0';
    return ESP_OK;
}

esp_err_t salina_http_get(const char *host, const char *path, const char *api_key, char *buf, size_t buf_size)
{
    ESP_RETURN_ON_FALSE(host && path && buf && buf_size, ESP_ERR_INVALID_ARG, TAG, "null argument");
    buf[0] = '\0';

    response_t resp = { .buf = buf, .len = 0, .cap = buf_size };
    const esp_http_client_config_t cfg = {
        .host = host,
        .path = path,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .event_handler = http_event,
        .user_data = &resp,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 10000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    ESP_RETURN_ON_FALSE(client, ESP_FAIL, TAG, "client init");

    if (api_key && api_key[0]) {
        esp_http_client_set_header(client, "X-Access-Token", api_key);
    }

    esp_err_t ret = esp_http_client_perform(client);
    if (ret == ESP_OK) {
        const int status = esp_http_client_get_status_code(client);
        if (status != 200) {
            ESP_LOGE(TAG, "HTTP %d from %s", status, host);
            ret = (status == 401 || status == 403) ? ESP_ERR_INVALID_STATE : ESP_ERR_INVALID_RESPONSE;
        }
    } else {
        ESP_LOGE(TAG, "request failed: %s", esp_err_to_name(ret));
    }
    esp_http_client_cleanup(client);
    return ret;
}

void salina_url_encode(const char *in, char *out, size_t out_size)
{
    static const char hex[] = "0123456789ABCDEF";
    size_t o = 0;

    for (const unsigned char *p = (const unsigned char *)in; *p && o + 4 < out_size; p++) {
        if (isalnum(*p) || *p == '-' || *p == '_' || *p == '.' || *p == '~') {
            out[o++] = (char)*p;
        } else {
            out[o++] = '%';
            out[o++] = hex[*p >> 4];
            out[o++] = hex[*p & 0x0F];
        }
    }
    out[o] = '\0';
}
