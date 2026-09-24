/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Provisioned settings, stored in NVS rather than compiled in.
 */
#pragma once

#include <stdbool.h>

#include "esp_err.h"

#define SALINA_SSID_LEN 33
#define SALINA_PASS_LEN 65
#define SALINA_STOP_LEN 64

typedef struct {
    char ssid[SALINA_SSID_LEN];
    char password[SALINA_PASS_LEN];
    char stop[SALINA_STOP_LEN];
    int sign_left;
    int sign_right;
} salina_config_t;

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

/* The open access point an unprovisioned board offers. */
#define SALINA_AP_SSID "esp_salina"
#define SALINA_AP_URL  "http://192.168.4.1"

/**
 * @brief Run the setup portal, returning only once the settings are saved.
 *
 * @return ESP_OK once written to NVS - the caller is expected to restart - or
 *         an error if the portal could not be brought up at all.
 */
esp_err_t salina_portal_run(void);
