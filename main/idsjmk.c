/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * IDS JMK: Brno and the rest of the South Moravian network.
 *
 * GET /api/departures/busstop-by-name?busStopName=<stop>
 *
 * The API matches loosely - case, diacritics and partial names all work - and
 * answers with a single best match rather than a list. Platforms are numbered,
 * and a platform with nothing due is omitted entirely, so an empty sign list
 * means "nothing soon" and not "no such platform". Times arrive either as
 * "7min" or as "16:03", optionally prefixed with the wheelchair symbol.
 */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "salina.h"
#include "salina_config.h"
#include "salina_http.h"

static const char *TAG = "idsjmk";

#define API_HOST "www.idsjmk.cz"
#define API_PATH "/api/departures/busstop-by-name?busStopName="

/** @brief True once the clock holds a real date rather than 1970. */
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

    /* A countdown is only true at the moment of the request; by the next
     * refresh it is stale, so it becomes the time the vehicle is expected. */
    const int minutes = atoi(in);
    if (!salina_clock_is_set()) {
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
    const cJSON *departure = NULL;

    col->count = 0;
    cJSON_ArrayForEach(departure, cJSON_GetObjectItemCaseSensitive(sign, "departures"))
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

/** @brief Build the request path for a stop name. */
static void stop_path(const char *stop, char *path, size_t path_size)
{
    char encoded[128];

    salina_url_encode(stop, encoded, sizeof(encoded));
    snprintf(path, path_size, "%s%s", API_PATH, encoded);
}

/** @brief The first stop of a reply, or NULL. */
static const cJSON *first_stop(const cJSON *root)
{
    return cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(root, "stops"), 0);
}

static esp_err_t parse_departures(const char *json, const salina_config_t *cfg, salina_data_t *out)
{
    cJSON *root = cJSON_Parse(json);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    const cJSON *entry = first_stop(root);
    if (!entry) {
        ESP_LOGE(TAG, "no stop named \"%s\"", cfg->stop);
        goto out;
    }

    const cJSON *stop = cJSON_GetObjectItemCaseSensitive(entry, "stop");
    const cJSON *name = cJSON_GetObjectItemCaseSensitive(stop, "chapsName");
    strlcpy(out->stop_name, cJSON_IsString(name) ? name->valuestring : cfg->stop, sizeof(out->stop_name));

    const char *wanted[SALINA_COLUMNS] = { cfg->left, cfg->right };
    for (int i = 0; i < SALINA_COLUMNS; i++) {
        strlcpy(out->columns[i].label, wanted[i], sizeof(out->columns[i].label));
        out->columns[i].count = 0;

        const cJSON *sign = NULL;
        cJSON_ArrayForEach(sign, cJSON_GetObjectItemCaseSensitive(entry, "signs"))
        {
            const cJSON *info = cJSON_GetObjectItemCaseSensitive(sign, "busStopSign");
            const cJSON *number = cJSON_GetObjectItemCaseSensitive(info, "number");
            if (cJSON_IsNumber(number) && number->valueint == atoi(wanted[i])) {
                parse_sign(sign, &out->columns[i]);
                break;
            }
        }
        if (out->columns[i].count == 0) {
            ESP_LOGW(TAG, "platform %s has no departures", wanted[i]);
        }
    }
    /* The API sometimes answers with the stop but no platforms at all. That is
     * not worth a screen refresh, and it clears on a retry moments later. */
    ret = (out->columns[0].count || out->columns[1].count) ? ESP_OK : ESP_ERR_NOT_FOUND;

out:
    cJSON_Delete(root);
    return ret;
}

static esp_err_t parse_stop_info(const char *json, const char *query, salina_stop_info_t *out)
{
    cJSON *root = cJSON_Parse(json);
    ESP_RETURN_ON_FALSE(root, ESP_ERR_INVALID_RESPONSE, TAG, "malformed JSON");

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    const cJSON *entry = first_stop(root);
    if (!entry) {
        goto out;
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
        snprintf(slot->selector, sizeof(slot->selector), "%d", number->valueint);
        strlcpy(slot->label, slot->selector, sizeof(slot->label));
        strlcpy(slot->description, cJSON_IsString(desc) ? desc->valuestring : "", sizeof(slot->description));
    }
    ret = ESP_OK;

out:
    cJSON_Delete(root);
    return ret;
}

static esp_err_t idsjmk_fetch(const salina_config_t *cfg, salina_data_t *out)
{
    ESP_RETURN_ON_FALSE(cfg && out, ESP_ERR_INVALID_ARG, TAG, "null argument");

    char path[256];
    stop_path(cfg->stop, path, sizeof(path));

    char *body = malloc(SALINA_RESPONSE_LIMIT);
    ESP_RETURN_ON_FALSE(body, ESP_ERR_NO_MEM, TAG, "no mem for response");

    esp_err_t ret = ESP_FAIL;
    for (int attempt = 1; attempt <= CONFIG_SALINA_FETCH_ATTEMPTS; attempt++) {
        ret = salina_http_get(API_HOST, path, NULL, body, SALINA_RESPONSE_LIMIT);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "got %u bytes", (unsigned)strlen(body));
            ret = parse_departures(body, cfg, out);
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

    free(body);
    return ret;
}

static esp_err_t idsjmk_lookup(const salina_config_t *cfg, const char *query, salina_stop_info_t *out)
{
    (void)cfg;
    ESP_RETURN_ON_FALSE(query && out, ESP_ERR_INVALID_ARG, TAG, "null argument");

    char path[256];
    stop_path(query, path, sizeof(path));

    char *body = malloc(SALINA_RESPONSE_LIMIT);
    ESP_RETURN_ON_FALSE(body, ESP_ERR_NO_MEM, TAG, "no mem for response");

    /* An empty platform list here reads as "this stop has no platforms", which
     * is indistinguishable from the intermittently empty answer, so retry. */
    esp_err_t ret = ESP_FAIL;
    for (int attempt = 1; attempt <= CONFIG_SALINA_FETCH_ATTEMPTS; attempt++) {
        memset(out, 0, sizeof(*out));
        ret = salina_http_get(API_HOST, path, NULL, body, SALINA_RESPONSE_LIMIT);
        if (ret == ESP_OK) {
            ret = parse_stop_info(body, query, out);
        }
        if ((ret == ESP_OK && out->sign_count > 0) || attempt == CONFIG_SALINA_FETCH_ATTEMPTS) {
            break;
        }
        ESP_LOGW(TAG,
                 "attempt %d/%d gave no platforms, retrying in %d s",
                 attempt,
                 CONFIG_SALINA_FETCH_ATTEMPTS,
                 CONFIG_SALINA_FETCH_RETRY_DELAY_S);
        vTaskDelay(pdMS_TO_TICKS(CONFIG_SALINA_FETCH_RETRY_DELAY_S * 1000));
    }

    free(body);
    return ret;
}

const salina_source_t salina_source_idsjmk = {
    .name = "Brno and South Moravia (IDS JMK)",
    .hint = "Accents and case do not matter and partial names work, so "
            "\"kartouzska\" finds \"Kartouzska\".",
    .needs_api_key = false,
    .lookup = idsjmk_lookup,
    .fetch = idsjmk_fetch,
};
