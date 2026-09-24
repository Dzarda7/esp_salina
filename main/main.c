/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Departure display for a Brno stop, on a LaskaKit ESPink-Shelf-2.9.
 *
 * Every wake-up is a full cycle: power the panel, join Wi-Fi, fetch the
 * departures, draw them, refresh the glass, and deep sleep again. The ESP32
 * keeps nothing but the clock between wakes, which is why every refresh is a
 * full one.
 *
 * If an update fails, the screen is deliberately left untouched - e-paper holds
 * its last image without power, so the board keeps showing the previous
 * departures instead of a blank or an error page. The fetch itself retries a
 * few times while Wi-Fi is up, so a failure here means the cadence is kept.
 */
#include <inttypes.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "salina.h"
#include "salina_config.h"

static const char *TAG = "salina";

/* The RTC drifts enough over days to matter, but a sync costs about 2.4 s of
 * radio-on time, which is more than the rest of the wake put together. Once a
 * day is the compromise. */
#define CLOCK_RESYNC_INTERVAL_S (24 * 3600)

/* Only used when the portal itself will not start; it holds open otherwise. */
#define PORTAL_RETRY_S          60

/* Survives deep sleep, lost on a power cycle. */
RTC_DATA_ATTR static bool s_clock_valid;
RTC_DATA_ATTR static time_t s_last_sync;
RTC_DATA_ATTR static uint32_t s_wake_count;

/** @brief Seconds to sleep: the configured interval, or until morning at night. */
static uint64_t sleep_seconds(const struct tm *now)
{
    if (!s_clock_valid) {
        /* Without a clock we cannot tell night from day; just keep the cadence. */
        return (uint64_t)CONFIG_SALINA_UPDATE_INTERVAL_MIN * 60;
    }

    const bool night =
        (CONFIG_SALINA_NIGHT_START_HOUR > CONFIG_SALINA_NIGHT_END_HOUR)
            ? (now->tm_hour >= CONFIG_SALINA_NIGHT_START_HOUR || now->tm_hour < CONFIG_SALINA_NIGHT_END_HOUR)
            : (now->tm_hour >= CONFIG_SALINA_NIGHT_START_HOUR && now->tm_hour < CONFIG_SALINA_NIGHT_END_HOUR);
    if (!night) {
        return (uint64_t)CONFIG_SALINA_UPDATE_INTERVAL_MIN * 60;
    }

    struct tm morning = *now;
    morning.tm_hour = CONFIG_SALINA_NIGHT_END_HOUR;
    morning.tm_min = 0;
    morning.tm_sec = 0;
    if (now->tm_hour >= CONFIG_SALINA_NIGHT_END_HOUR) {
        morning.tm_mday += 1; /* mktime() normalises the month roll-over */
    }
    const time_t wake_at = mktime(&morning);
    const time_t now_t = mktime((struct tm *)now);
    const double delta = difftime(wake_at, now_t);

    ESP_LOGI(TAG, "night sleep until %02d:00", CONFIG_SALINA_NIGHT_END_HOUR);
    return (delta > 0) ? (uint64_t)delta : (uint64_t)CONFIG_SALINA_UPDATE_INTERVAL_MIN * 60;
}

static void enter_deep_sleep(uint64_t seconds)
{
    ESP_LOGI(TAG, "sleeping for %llu s", (unsigned long long)seconds);

    /* Drive the rail down here rather than relying on the panel driver having
     * claimed the pin: a wake that failed before the display came up never
     * called the power hook, and holding an unconfigured pin would leave the
     * panel powered for the whole sleep. The hook configures the pin itself. */
    board_panel_power(false, NULL);
    gpio_hold_en(BOARD_PIN_POWER);
    gpio_deep_sleep_hold_en();

    esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
    esp_deep_sleep_start();
}

/**
 * @brief Show the setup instructions, then serve the setup page.
 *
 * The screen is drawn first and the rail cut again before the radio starts:
 * this board browns out if the panel supply and a transmitting Wi-Fi overlap.
 * Returns only if the portal could not be started.
 */
static void run_setup_portal(void)
{
    if (salina_ui_init() == ESP_OK) {
        salina_ui_show_portal(SALINA_AP_SSID, SALINA_AP_URL);
        salina_ui_sleep();
        salina_ui_release();
    }
    /* Cut the rail, not just sleep the controller: an access point transmits
     * far more than a station ever does, and the two supplies together collapse
     * this board. E-paper keeps the image with no power at all. */
    board_panel_power(false, NULL);
    ESP_LOGW(TAG, "not provisioned, starting the setup portal");

    if (salina_portal_run() == ESP_OK) {
        ESP_LOGI(TAG, "provisioned, restarting");
        esp_restart();
    }
}

void app_main(void)
{
    /* Nothing from here on may abort: a panic reboots straight into another
     * wake, and a board that cannot sleep flattens its cell in a day. Every
     * failure falls through to the same deep sleep as a good wake. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        if (nvs_flash_erase() == ESP_OK) {
            err = nvs_flash_init();
        }
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs init: %s", esp_err_to_name(err));
    }

    /* The pin is held from the previous sleep; release it before driving it. */
    gpio_hold_dis(BOARD_PIN_POWER);
    gpio_deep_sleep_hold_dis();

    salina_time_set_tz();

    /* An unprovisioned board has nothing to show and nowhere to fetch it from,
     * so it puts up its own access point and waits to be told. */
    if (salina_config_pin_asserted()) {
        ESP_LOGW(TAG, "config button held, forgetting the stored settings");
        salina_config_erase();
    }

    salina_config_t cfg = { 0 };
    if (salina_config_load(&cfg) != ESP_OK) {
        run_setup_portal();
        /* Only reached if the portal could not start; sleep briefly and retry
         * rather than spin through reboots. */
        enter_deep_sleep(PORTAL_RETRY_S);
    }

    ESP_LOGI(TAG, "wake %" PRIu32 ", stop \"%s\"", ++s_wake_count, cfg.stop);

    salina_data_t data = { 0 };
    bool have_data = false;

    if (salina_net_connect(cfg.ssid, cfg.password) == ESP_OK) {
        /* A failed sync must not cost us the update, so the old clock stands. */
        const bool stale = difftime(time(NULL), s_last_sync) >= CLOCK_RESYNC_INTERVAL_S;
        if ((!s_clock_valid || stale) && salina_time_sync() == ESP_OK) {
            s_clock_valid = true;
            s_last_sync = time(NULL);
        }
        have_data = (salina_fetch_departures(&cfg, &data) == ESP_OK);
    }
    salina_net_disconnect();

    /* The panel rail is only powered once the radio is off again: running both
     * at once sags the supply enough to trip the brownout detector. */
    const esp_err_t ui_err = salina_ui_init();
    if (ui_err != ESP_OK) {
        ESP_LOGE(TAG, "display unavailable: %s", esp_err_to_name(ui_err));
    }

    const time_t now_t = time(NULL);
    struct tm now;
    localtime_r(&now_t, &now);

    if (have_data && ui_err == ESP_OK) {
        int battery_mv = -1;
        salina_battery_read(&battery_mv, NULL);
        const esp_err_t render_err = salina_ui_render(&data, &now, battery_mv);
        if (render_err != ESP_OK) {
            ESP_LOGE(TAG, "render: %s", esp_err_to_name(render_err));
        }
    } else {
        /* Leave the old image on the glass and wait for the next wake. */
        ESP_LOGW(TAG, "update failed, keeping the previous screen");
    }

    salina_ui_sleep();
    enter_deep_sleep(sleep_seconds(&now));
}
