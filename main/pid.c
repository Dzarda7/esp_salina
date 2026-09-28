/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Prague Integrated Transport, through the Golemio API.
 *
 * GET /v2/pid/departureboards?names=<stop>   - the stop and all its platforms
 * GET /v2/pid/departureboards?ids=<stop_id>  - departures for one platform
 *
 * Three things drive the shape of this file:
 *
 *   - Stop names must be exact, including case and diacritics. The API has no
 *     substring search of any kind, and the stop list is megabytes, so there
 *     is nothing to be done except pass on what was typed.
 *   - `limit` counts departures across the whole reply, not per platform, so
 *     asking for both columns at once lets a busy platform starve the other.
 *     Each column is therefore its own request.
 *   - Platforms are labelled with letters as well as digits ("A", "J", "1"),
 *     so the column selector is the GTFS stop id rather than the label.
 *
 * Times come back as ISO timestamps already in Prague local time, so the wall
 * clock is a substring and needs no conversion.
 */
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "salina.h"
#include "salina_config.h"
#include "salina_http.h"

static const char *TAG = "pid";

#define API_HOST       "api.golemio.cz"
#define API_PATH       "/v2/pid/departureboards"
/* Five rows is what the panel shows; asking for more only costs buffer. */
#define PLATFORM_LIMIT 5

/** @brief "2026-09-28T16:13:36+02:00" -> "16:13". */
static void format_time(const char *iso, char *out, size_t out_size)
{
    if (strlen(iso) < 16) {
        strlcpy(out, iso, out_size);
        return;
    }
    snprintf(out, out_size, "%.5s", iso + 11);
}

static void parse_departures(const cJSON *root, salina_column_t *col)
{
    const cJSON *departure = NULL;

    col->count = 0;
    cJSON_ArrayForEach(departure, cJSON_GetObjectItemCaseSensitive(root, "departures"))
    {
        if (col->count >= SALINA_MAX_DEPARTURES) {
            break;
        }
        const cJSON *route = cJSON_GetObjectItemCaseSensitive(departure, "route");
        const cJSON *trip = cJSON_GetObjectItemCaseSensitive(departure, "trip");
        const cJSON *stamp = cJSON_GetObjectItemCaseSensitive(departure, "departure_timestamp");
        const cJSON *line = cJSON_GetObjectItemCaseSensitive(route, "short_name");
        const cJSON *dest = cJSON_GetObjectItemCaseSensitive(trip, "headsign");
        const cJSON *predicted = cJSON_GetObjectItemCaseSensitive(stamp, "predicted");
        const cJSON *scheduled = cJSON_GetObjectItemCaseSensitive(stamp, "scheduled");
        const cJSON *when = cJSON_IsString(predicted) ? predicted : scheduled;
        if (!cJSON_IsString(line) || !cJSON_IsString(dest) || !cJSON_IsString(when)) {
            continue;
        }

        salina_departure_t *d = &col->departures[col->count++];
        strlcpy(d->line, line->valuestring, sizeof(d->line));
        strlcpy(d->destination, dest->valuestring, sizeof(d->destination));
        format_time(when->valuestring, d->when, sizeof(d->when));
        /* "Real time available" is the closest thing to the online flag the
         * other source reports. */
        const cJSON *delay = cJSON_GetObjectItemCaseSensitive(departure, "delay");
        d->online = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(delay, "is_available"));
    }
}

/** @brief Ask for one platform by its GTFS stop id. */
static esp_err_t fetch_platform(const salina_config_t *cfg, const char *stop_id, char *body, salina_column_t *col)
{
    char encoded[SALINA_SELECTOR_LEN * 3];
    salina_url_encode(stop_id, encoded, sizeof(encoded));

    char path[160];
    snprintf(path, sizeof(path), "%s?ids=%s&limit=%d", API_PATH, encoded, PLATFORM_LIMIT);

    ESP_RETURN_ON_ERROR(salina_http_get(API_HOST, path, cfg->api_key, body, SALINA_RESPONSE_LIMIT),
                        TAG,
                        "platform %s",
                        stop_id);

    cJSON *root = cJSON_Parse(body);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    /* The reply repeats the platform, which is where the label comes from. */
    const cJSON *stop = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(root, "stops"), 0);
    const cJSON *code = cJSON_GetObjectItemCaseSensitive(stop, "platform_code");
    strlcpy(col->label, cJSON_IsString(code) ? code->valuestring : "", sizeof(col->label));

    const cJSON *name = cJSON_GetObjectItemCaseSensitive(stop, "stop_name");
    parse_departures(root, col);
    const bool named = cJSON_IsString(name);
    if (named) {
        strlcpy(body, name->valuestring, SALINA_STOP_LEN); /* handed back to the caller */
    }
    cJSON_Delete(root);
    return named ? ESP_OK : ESP_ERR_NOT_FOUND;
}

static esp_err_t pid_fetch(const salina_config_t *cfg, salina_data_t *out)
{
    ESP_RETURN_ON_FALSE(cfg && out, ESP_ERR_INVALID_ARG, TAG, "null argument");
    ESP_RETURN_ON_FALSE(cfg->api_key[0], ESP_ERR_INVALID_STATE, TAG, "no API key stored");

    char *body = malloc(SALINA_RESPONSE_LIMIT);
    ESP_RETURN_ON_FALSE(body, ESP_ERR_NO_MEM, TAG, "no mem for response");

    const char *wanted[SALINA_COLUMNS] = { cfg->left, cfg->right };
    strlcpy(out->stop_name, cfg->stop, sizeof(out->stop_name));

    esp_err_t ret = ESP_FAIL;
    for (int attempt = 1; attempt <= CONFIG_SALINA_FETCH_ATTEMPTS; attempt++) {
        for (int i = 0; i < SALINA_COLUMNS; i++) {
            out->columns[i].count = 0;
            if (fetch_platform(cfg, wanted[i], body, &out->columns[i]) == ESP_OK) {
                strlcpy(out->stop_name, body, sizeof(out->stop_name));
            }
        }
        ret = (out->columns[0].count || out->columns[1].count) ? ESP_OK : ESP_ERR_NOT_FOUND;
        if (ret == ESP_OK || attempt == CONFIG_SALINA_FETCH_ATTEMPTS) {
            break;
        }
        ESP_LOGW(TAG,
                 "attempt %d/%d found nothing, retrying in %d s",
                 attempt,
                 CONFIG_SALINA_FETCH_ATTEMPTS,
                 CONFIG_SALINA_FETCH_RETRY_DELAY_S);
        vTaskDelay(pdMS_TO_TICKS(CONFIG_SALINA_FETCH_RETRY_DELAY_S * 1000));
    }

    free(body);
    return ret;
}

static esp_err_t pid_lookup(const salina_config_t *cfg, const char *query, salina_stop_info_t *out)
{
    ESP_RETURN_ON_FALSE(cfg && query && out, ESP_ERR_INVALID_ARG, TAG, "null argument");
    ESP_RETURN_ON_FALSE(cfg->api_key[0], ESP_ERR_INVALID_STATE, TAG, "no API key given");
    memset(out, 0, sizeof(*out));

    char encoded[SALINA_STOP_LEN * 3];
    salina_url_encode(query, encoded, sizeof(encoded));

    /* One departure is enough: this call is only after the platform list. */
    char path[256];
    snprintf(path, sizeof(path), "%s?names=%s&limit=1", API_PATH, encoded);

    char *body = malloc(SALINA_RESPONSE_LIMIT);
    ESP_RETURN_ON_FALSE(body, ESP_ERR_NO_MEM, TAG, "no mem for response");

    esp_err_t ret = salina_http_get(API_HOST, path, cfg->api_key, body, SALINA_RESPONSE_LIMIT);
    if (ret != ESP_OK) {
        free(body);
        return ret;
    }

    cJSON *root = cJSON_Parse(body);
    free(body);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    strlcpy(out->name, query, sizeof(out->name));
    strlcpy(out->full_name, query, sizeof(out->full_name));

    const cJSON *stop = NULL;
    cJSON_ArrayForEach(stop, cJSON_GetObjectItemCaseSensitive(root, "stops"))
    {
        if (out->sign_count >= SALINA_MAX_SIGNS) {
            break;
        }
        const cJSON *id = cJSON_GetObjectItemCaseSensitive(stop, "stop_id");
        const cJSON *code = cJSON_GetObjectItemCaseSensitive(stop, "platform_code");
        const cJSON *name = cJSON_GetObjectItemCaseSensitive(stop, "stop_name");
        if (!cJSON_IsString(id)) {
            continue;
        }
        if (cJSON_IsString(name)) {
            strlcpy(out->name, name->valuestring, sizeof(out->name));
            strlcpy(out->full_name, name->valuestring, sizeof(out->full_name));
        }
        salina_sign_t *slot = &out->signs[out->sign_count++];
        strlcpy(slot->selector, id->valuestring, sizeof(slot->selector));
        strlcpy(slot->label, cJSON_IsString(code) ? code->valuestring : "?", sizeof(slot->label));
        /* PID gives no direction text for a platform; the label is all there is. */
        slot->description[0] = '\0';
    }

    cJSON_Delete(root);
    return (out->sign_count > 0) ? ESP_OK : ESP_ERR_NOT_FOUND;
}

const salina_source_t salina_source_pid = {
    .name = "Prague (PID)",
    .hint = "The name must be exact, including capitals and accents: "
            "\"Malostranska\" works, \"malostranska\" does not.",
    .needs_api_key = true,
    .key_hint = "Free registration at api.golemio.cz/api-keys. Keys are per person, "
                "so use your own rather than sharing one.",
    .lookup = pid_lookup,
    .fetch = pid_fetch,
};
