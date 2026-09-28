/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Provisioned settings, stored in NVS rather than compiled in.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "esp_err.h"

#define SALINA_SSID_LEN     33
#define SALINA_PASS_LEN     65
#define SALINA_STOP_LEN     64
#define SALINA_SELECTOR_LEN 24
/* Golemio hands out a JWT; the one this was written against is 224 characters. */
#define SALINA_API_KEY_LEN  320

/** @brief Which network the departures come from. */
typedef enum {
    SALINA_SOURCE_IDSJMK = 0, /*!< Brno and the rest of the IDS JMK area */
    SALINA_SOURCE_PID,        /*!< Prague Integrated Transport, via Golemio */
    SALINA_SOURCE_COUNT,
} salina_source_id_t;

typedef struct {
    char ssid[SALINA_SSID_LEN];
    char password[SALINA_PASS_LEN];
    salina_source_id_t source;
    char stop[SALINA_STOP_LEN];
    /* What each column asks the source for. IDS JMK numbers its platforms, so
     * this is "1"; PID has no usable platform key, so it is a stop id such as
     * "U360Z1P". Kept as text so one field serves both. */
    char left[SALINA_SELECTOR_LEN];
    char right[SALINA_SELECTOR_LEN];
    char api_key[SALINA_API_KEY_LEN]; /*!< Empty for sources that need none. */
} salina_config_t;

/* The open access point an unprovisioned board offers. */
#define SALINA_AP_SSID "esp_salina"
#define SALINA_AP_URL  "http://192.168.4.1"

/**
 * @brief Read the provisioned settings.
 *
 * @return ESP_ERR_NVS_NOT_FOUND when the board has never been provisioned,
 *         which is the caller's cue to run the setup portal.
 */
esp_err_t salina_config_load(salina_config_t *out);
esp_err_t salina_config_save(const salina_config_t *cfg);
esp_err_t salina_config_erase(void);

/** @brief True when the optional config button is held down at boot. */
bool salina_config_pin_asserted(void);

/**
 * @brief Run the setup portal, returning only once the settings are saved.
 *
 * @return ESP_OK once written to NVS - the caller is expected to restart - or
 *         an error if the portal could not be brought up at all.
 */
esp_err_t salina_portal_run(void);
