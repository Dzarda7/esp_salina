/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Wiring of the LaskaKit ESPink-Shelf-2.9 (ESP32 + GDEY029T94).
 *
 * GPIO2 switches the 3V3 rail feeding the display, so it has to be high before
 * the panel answers and is pulled low before deep sleep.
 */
#pragma once

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_lcd_epaper.h"

#define BOARD_SPI_HOST            SPI2_HOST
#define BOARD_PIN_MOSI            23
#define BOARD_PIN_SCLK            18
#define BOARD_PIN_CS              5
#define BOARD_PIN_DC              17
#define BOARD_PIN_RST             16
#define BOARD_PIN_BUSY            4
#define BOARD_PIN_POWER           2
#define BOARD_PIXEL_CLK_HZ        (10 * 1000 * 1000)

/* Which display is attached and which way up it sits. Swapping in another
 * e-paper panel means changing these two lines and the pins above - the rest of
 * the application derives its layout from whatever size the driver reports. */
#define BOARD_EPD_PANEL           (&epd_panel_gdey029t94)
#define BOARD_EPD_ROTATION        EPD_ROTATION_90

/* Battery sense: GPIO34 is ADC1 channel 6 on the ESP32. */
#define BOARD_BATTERY_ADC_UNIT    ADC_UNIT_1
#define BOARD_BATTERY_ADC_CHANNEL ADC_CHANNEL_6

/**
 * @brief Power hook handed to the e-paper driver.
 *
 * Lives in board.c rather than here: it claims GPIO 2 on first use, and a
 * static inline would give every translation unit its own copy of that guard,
 * so the pin would be configured once per caller and the GPIO driver would
 * report a conflict.
 */
esp_err_t board_panel_power(bool on, void *user_ctx);
