/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Shared types of the departure display.
 */
#pragma once

#include <stdbool.h>
#include <time.h>

#include "esp_err.h"
#include "salina_config.h"

/* Upper bound on what is kept from the API; how many actually fit on screen
 * depends on the panel and is worked out at runtime. */
#define SALINA_MAX_DEPARTURES 12
#define SALINA_COLUMNS        2

/** @brief One departure row as shown on screen. */
typedef struct {
    char line[6];         /*!< Line number, e.g. "6" or "N95" */
    char destination[64]; /*!< Where it goes, e.g. "Královo Pole, nádraží" */
    char when[8];         /*!< "7'" for minutes, or "16:03" for a wall clock time */
    bool online;          /*!< true when the API reports a real-time position */
} salina_departure_t;

/** @brief One platform of the stop, shown as one column. */
typedef struct {
    int sign_number;
    int count;
    salina_departure_t departures[SALINA_MAX_DEPARTURES];
} salina_column_t;

/** @brief Everything the screen needs. */
typedef struct {
    char stop_name[64];
    salina_column_t columns[SALINA_COLUMNS];
} salina_data_t;

/* --- network (net.c) --- */
esp_err_t salina_net_connect(const char *ssid, const char *password);
void salina_net_disconnect(void);
/** @brief Sync the clock over SNTP. */
esp_err_t salina_time_sync(void);
/** @brief Apply the local timezone; the setting does not survive deep sleep. */
void salina_time_set_tz(void);
/** @brief True once the clock holds a real date rather than 1970. */
bool salina_clock_is_set(void);

/* --- departure source (idsjmk.c) ---
 *
 * The only part of the application that knows about IDS JMK. Serving another
 * city means replacing this one file with something that fills the same struct;
 * the display, the sleep policy and the layout are unaffected. */
esp_err_t salina_fetch_departures(const salina_config_t *cfg, salina_data_t *out);

#define SALINA_MAX_SIGNS 12

/** @brief One platform of a stop, as offered during setup. */
typedef struct {
    int number;
    char description[48]; /*!< Direction, e.g. ">Reckovice" */
} salina_sign_t;

/** @brief What a stop search turned up. */
typedef struct {
    char name[64];      /*!< As shown on the panel. Inside Brno the town is left
                         *   off ("Kartouzska"); elsewhere it is part of the
                         *   name ("Znojmo, Luzicka"). */
    char full_name[64]; /*!< Always carries the town, so setup can show which
                         *   stop a loose query actually matched. */
    salina_sign_t signs[SALINA_MAX_SIGNS];
    int sign_count;
} salina_stop_info_t;

/**
 * @brief Resolve a stop name to its real name and platforms.
 *
 * Covers the whole IDS JMK area, not only Brno. The API matches partially and
 * ignores case and diacritics, so "kartouzska" finds "Kartouzska". It answers
 * with a single best match, not a list, and it omits platforms that have no
 * upcoming departures - at night the sign list can come back empty.
 */
esp_err_t salina_lookup_stop(const char *query, salina_stop_info_t *out);

/* --- battery (battery.c) --- */
esp_err_t salina_battery_read(int *millivolts, int *percent);

/* --- display (ui.c) --- */
esp_err_t salina_ui_init(void);
/**
 * @brief Compose the screen and push it to the glass.
 *
 * @param[in] now         Time of this update; it is shown as the "last update"
 *                        stamp, and only advances when an update succeeds
 * @param[in] battery_mv  Battery voltage in mV, or -1 to leave it blank
 */
esp_err_t salina_ui_render(const salina_data_t *data, const struct tm *now, int battery_mv);
/** @brief Draw the setup instructions for an unprovisioned board. */
esp_err_t salina_ui_show_portal(const char *ssid, const char *url);
/**
 * @brief Give back everything the display holds.
 *
 * Used by the setup portal, which needs the memory for the radio and TLS.
 */
void salina_ui_release(void);
/** @brief Deep sleep the controller and cut the panel rail. */
void salina_ui_sleep(void);
