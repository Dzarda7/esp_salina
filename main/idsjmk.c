/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * IDS JMK departures API.
 *
 * GET https://www.idsjmk.cz/api/departures/busstop-by-name?busStopName=<stop>
 *
 * The response is small (under 2 kB for a tram stop), so it is buffered whole
 * and handed to cJSON. Interesting bits:
 *   stops[0].stop.chapsName          - the stop name
 *   stops[0].signs[n].busStopSign.number - platform number, selects a column
 *   stops[0].signs[n].departures[]   - link, destinationStop, time, isOnline
 *
 * Times arrive either as "7min" or as "16:03", optionally prefixed with U+267F
 * (the wheelchair symbol) for a low-floor vehicle, which is stripped here.
 */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"
#include "esp_check.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "salina.h"
#include "salina_config.h"

static const char *TAG = "idsjmk";

#define API_HOST       "www.idsjmk.cz"
#define API_PATH       "/api/departures/busstop-by-name?busStopName="
#define API_IDS_PATH   "/api/departures/busstops"
#define RESPONSE_LIMIT 8192

typedef struct {
    char *buf;
    size_t len;
} response_t;

/** @brief Percent-encode everything that is not an unreserved URL character. */
static void url_encode(const char *in, char *out, size_t out_size)
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

static esp_err_t http_event(esp_http_client_event_t *evt)
{
    response_t *resp = (response_t *)evt->user_data;

    if (evt->event_id != HTTP_EVENT_ON_DATA || !resp) {
        return ESP_OK;
    }
    if (resp->len + evt->data_len >= RESPONSE_LIMIT) {
        ESP_LOGE(TAG, "response larger than %d bytes", RESPONSE_LIMIT);
        return ESP_FAIL;
    }
    memcpy(resp->buf + resp->len, evt->data, evt->data_len);
    resp->len += evt->data_len;
    resp->buf[resp->len] = '\0';
    return ESP_OK;
}

/**
 * @brief Turn an API time string into the wall clock time of the departure.
 *
 * The API answers either "16:03", which is already what we want, or a countdown
 * like "7min", which is only true at the moment of the request - by the time the
 * panel refreshes again it is stale. Countdowns are therefore converted to the
 * absolute time the vehicle is expected. Both forms may carry a leading U+267F
 * (wheelchair) marking a low-floor vehicle, which is dropped.
 */
static void format_time(const char *in, char *out, size_t out_size)
{
    /* Skip anything before the first digit: that is the wheelchair glyph. */
    while (*in && !isdigit((unsigned char)*in)) {
        in++;
    }

    const char *min = strstr(in, "min");
    if (!min) {
        strlcpy(out, in, out_size); /* already a wall clock time */
        return;
    }

    const int minutes = atoi(in);
    if (!salina_clock_is_set()) {
        /* No synced clock, so an absolute time would be a lie. */
        snprintf(out, out_size, "%d'", minutes);
        return;
    }

    const time_t departure = time(NULL) + (time_t)minutes * 60;
    struct tm tm_departure;
    localtime_r(&departure, &tm_departure);
    strftime(out, out_size, "%H:%M", &tm_departure);
}

static void parse_sign(const cJSON *sign, salina_column_t *col)
{
    const cJSON *departures = cJSON_GetObjectItemCaseSensitive(sign, "departures");
    const cJSON *departure = NULL;

    col->count = 0;
    cJSON_ArrayForEach(departure, departures)
    {
        if (col->count >= SALINA_MAX_DEPARTURES) {
            break;
        }
        const cJSON *link = cJSON_GetObjectItemCaseSensitive(departure, "link");
        const cJSON *dest = cJSON_GetObjectItemCaseSensitive(departure, "destinationStop");
        const cJSON *time = cJSON_GetObjectItemCaseSensitive(departure, "time");
        const cJSON *online = cJSON_GetObjectItemCaseSensitive(departure, "isOnline");
        if (!cJSON_IsString(link) || !cJSON_IsString(dest) || !cJSON_IsString(time)) {
            continue;
        }

        salina_departure_t *d = &col->departures[col->count++];
        strlcpy(d->line, link->valuestring, sizeof(d->line));
        strlcpy(d->destination, dest->valuestring, sizeof(d->destination));
        format_time(time->valuestring, d->when, sizeof(d->when));
        d->online = cJSON_IsTrue(online);
    }
}

static esp_err_t parse_response(const char *json, const salina_config_t *cfg, salina_data_t *out)
{
    cJSON *root = cJSON_Parse(json);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    const cJSON *stops = cJSON_GetObjectItemCaseSensitive(root, "stops");
    const cJSON *stop_entry = cJSON_GetArrayItem(stops, 0);
    if (!stop_entry) {
        ESP_LOGE(TAG, "no stop named \"%s\"", cfg->stop);
        goto out;
    }

    const cJSON *stop = cJSON_GetObjectItemCaseSensitive(stop_entry, "stop");
    const cJSON *name = cJSON_GetObjectItemCaseSensitive(stop, "chapsName");
    strlcpy(out->stop_name, cJSON_IsString(name) ? name->valuestring : cfg->stop, sizeof(out->stop_name));

    const int wanted[SALINA_COLUMNS] = { cfg->sign_left, cfg->sign_right };
    const cJSON *signs = cJSON_GetObjectItemCaseSensitive(stop_entry, "signs");

    for (int i = 0; i < SALINA_COLUMNS; i++) {
        out->columns[i].sign_number = wanted[i];
        out->columns[i].count = 0;

        const cJSON *sign = NULL;
        cJSON_ArrayForEach(sign, signs)
        {
            const cJSON *info = cJSON_GetObjectItemCaseSensitive(sign, "busStopSign");
            const cJSON *number = cJSON_GetObjectItemCaseSensitive(info, "number");
            if (cJSON_IsNumber(number) && number->valueint == wanted[i]) {
                parse_sign(sign, &out->columns[i]);
                break;
            }
        }
        if (out->columns[i].count == 0) {
            ESP_LOGW(TAG, "sign %d has no departures", wanted[i]);
        }
    }
    /* The API sometimes answers with the stop but no platforms at all. That is
     * not worth a screen refresh, and it clears on a retry moments later. */
    ret = (out->columns[0].count || out->columns[1].count) ? ESP_OK : ESP_ERR_NOT_FOUND;

out:
    cJSON_Delete(root);
    return ret;
}

/**
 * @brief Perform one API request into a caller-provided buffer.
 *
 * @param[in]  path     Request path, query string included
 * @param[in]  body     JSON body for a POST, or NULL for a GET
 * @param[out] resp     Receives the response text
 */
static esp_err_t api_request(const char *path, const char *body, response_t *resp)
{
    const esp_http_client_config_t cfg = {
        .host = API_HOST,
        .path = path,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .event_handler = http_event,
        .user_data = resp,
        .method = body ? HTTP_METHOD_POST : HTTP_METHOD_GET,
        .timeout_ms = 10000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    ESP_RETURN_ON_FALSE(client, ESP_FAIL, TAG, "client init");

    if (body) {
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_post_field(client, body, strlen(body));
    }

    esp_err_t ret = esp_http_client_perform(client);
    if (ret == ESP_OK) {
        const int status = esp_http_client_get_status_code(client);
        if (status != 200) {
            ESP_LOGE(TAG, "HTTP %d", status);
            ret = ESP_ERR_INVALID_RESPONSE;
        }
    } else {
        ESP_LOGE(TAG, "request failed: %s", esp_err_to_name(ret));
    }
    esp_http_client_cleanup(client);
    return ret;
}

esp_err_t salina_fetch_departures(const salina_config_t *cfg, salina_data_t *out)
{
    ESP_RETURN_ON_FALSE(cfg && out, ESP_ERR_INVALID_ARG, TAG, "null argument");

    char encoded[128];
    url_encode(cfg->stop, encoded, sizeof(encoded));

    char path[sizeof(API_PATH) + sizeof(encoded)];
    snprintf(path, sizeof(path), "%s%s", API_PATH, encoded);

    response_t resp = { .buf = malloc(RESPONSE_LIMIT), .len = 0 };
    ESP_RETURN_ON_FALSE(resp.buf, ESP_ERR_NO_MEM, TAG, "no mem for response");

    esp_err_t ret = ESP_FAIL;
    for (int attempt = 1; attempt <= CONFIG_SALINA_FETCH_ATTEMPTS; attempt++) {
        resp.len = 0;
        resp.buf[0] = '\0';

        ret = api_request(path, NULL, &resp);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "got %u bytes", (unsigned)resp.len);
            ret = parse_response(resp.buf, cfg, out);
        }
        if (ret == ESP_OK || attempt == CONFIG_SALINA_FETCH_ATTEMPTS) {
            break;
        }

        ESP_LOGW(TAG,
                 "attempt %d/%d failed, retrying in %d s",
                 attempt,
                 CONFIG_SALINA_FETCH_ATTEMPTS,
                 CONFIG_SALINA_FETCH_RETRY_DELAY_S);
        vTaskDelay(pdMS_TO_TICKS(CONFIG_SALINA_FETCH_RETRY_DELAY_S * 1000));
    }

    free(resp.buf);
    return ret;
}

esp_err_t salina_lookup_stop(const char *query, salina_stop_info_t *out)
{
    ESP_RETURN_ON_FALSE(query && out, ESP_ERR_INVALID_ARG, TAG, "null argument");
    memset(out, 0, sizeof(*out));

    char encoded[128];
    url_encode(query, encoded, sizeof(encoded));

    char path[sizeof(API_PATH) + sizeof(encoded)];
    snprintf(path, sizeof(path), "%s%s", API_PATH, encoded);

    response_t resp = { .buf = malloc(RESPONSE_LIMIT), .len = 0 };
    ESP_RETURN_ON_FALSE(resp.buf, ESP_ERR_NO_MEM, TAG, "no mem for response");
    resp.buf[0] = '\0';

    esp_err_t ret = api_request(path, NULL, &resp);
    if (ret != ESP_OK) {
        free(resp.buf);
        return ret;
    }

    cJSON *root = cJSON_Parse(resp.buf);
    free(resp.buf);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    const cJSON *stops = cJSON_GetObjectItemCaseSensitive(root, "stops");
    const cJSON *entry = cJSON_GetArrayItem(stops, 0);
    if (!entry) {
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    const cJSON *stop = cJSON_GetObjectItemCaseSensitive(entry, "stop");
    const cJSON *name = cJSON_GetObjectItemCaseSensitive(stop, "chapsName");
    const cJSON *full = cJSON_GetObjectItemCaseSensitive(stop, "fullName");
    strlcpy(out->name, cJSON_IsString(name) ? name->valuestring : query, sizeof(out->name));
    strlcpy(out->full_name, cJSON_IsString(full) ? full->valuestring : out->name, sizeof(out->full_name));

    const cJSON *sign = NULL;
    cJSON_ArrayForEach(sign, cJSON_GetObjectItemCaseSensitive(entry, "signs"))
    {
        if (out->sign_count >= SALINA_MAX_SIGNS) {
            break;
        }
        const cJSON *info = cJSON_GetObjectItemCaseSensitive(sign, "busStopSign");
        const cJSON *number = cJSON_GetObjectItemCaseSensitive(info, "number");
        const cJSON *desc = cJSON_GetObjectItemCaseSensitive(info, "description");
        if (!cJSON_IsNumber(number)) {
            continue;
        }
        salina_sign_t *slot = &out->signs[out->sign_count++];
        slot->number = number->valueint;
        strlcpy(slot->description, cJSON_IsString(desc) ? desc->valuestring : "", sizeof(slot->description));
    }

    cJSON_Delete(root);
    return ESP_OK;
}
