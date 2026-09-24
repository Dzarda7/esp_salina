/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Battery voltage on GPIO34 through the board's resistor divider.
 *
 * The percentage is a crude linear map between the configured empty and full
 * voltages - good enough for a four-step battery glyph, not a fuel gauge.
 */
#include "board.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_log.h"
#include "salina.h"

static const char *TAG = "battery";

#define BATTERY_SAMPLES 16

esp_err_t salina_battery_read(int *millivolts, int *percent)
{
    adc_oneshot_unit_handle_t adc = NULL;
    const adc_oneshot_unit_init_cfg_t unit_cfg = { .unit_id = BOARD_BATTERY_ADC_UNIT };
    ESP_RETURN_ON_ERROR(adc_oneshot_new_unit(&unit_cfg, &adc), TAG, "adc unit");

    esp_err_t ret = ESP_OK;
    adc_cali_handle_t cali = NULL;
    const adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12, /* full scale ~3.1 V, the divider keeps us under it */
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_GOTO_ON_ERROR(adc_oneshot_config_channel(adc, BOARD_BATTERY_ADC_CHANNEL, &chan_cfg), out, TAG, "adc channel");

    const adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = BOARD_BATTERY_ADC_UNIT,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    const bool calibrated = adc_cali_create_scheme_line_fitting(&cali_cfg, &cali) == ESP_OK;

    int sum = 0;
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
        int raw = 0;
        ESP_GOTO_ON_ERROR(adc_oneshot_read(adc, BOARD_BATTERY_ADC_CHANNEL, &raw), out, TAG, "adc read");
        sum += raw;
    }
    const int raw_avg = sum / BATTERY_SAMPLES;

    int mv_at_pin = 0;
    if (calibrated) {
        ESP_GOTO_ON_ERROR(adc_cali_raw_to_voltage(cali, raw_avg, &mv_at_pin), out, TAG, "calibrate");
    } else {
        /* Uncalibrated fallback: 12 bit over the ~3100 mV full scale. */
        mv_at_pin = raw_avg * 3100 / 4095;
        ESP_LOGW(TAG, "no ADC calibration, voltage is approximate");
    }

    const int mv = mv_at_pin * CONFIG_SALINA_BATTERY_DIVIDER_X100 / 100;
    int pct =
        (mv - CONFIG_SALINA_BATTERY_EMPTY_MV) * 100 / (CONFIG_SALINA_BATTERY_FULL_MV - CONFIG_SALINA_BATTERY_EMPTY_MV);
    if (pct < 0) {
        pct = 0;
    } else if (pct > 100) {
        pct = 100;
    }

    ESP_LOGI(TAG, "%d mV (%d%%)", mv, pct);
    if (millivolts) {
        *millivolts = mv;
    }
    if (percent) {
        *percent = pct;
    }

out:
    if (cali) {
        adc_cali_delete_scheme_line_fitting(cali);
    }
    adc_oneshot_del_unit(adc);
    return ret;
}
