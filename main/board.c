/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Board level control that needs to exist exactly once.
 */
#include "board.h"

esp_err_t board_panel_power(bool on, void *user_ctx)
{
    (void)user_ctx;
    /* Claim the pin once: a second gpio_config() on a pin that is already
     * configured makes the GPIO driver report a conflict. */
    static bool configured;

    if (!configured) {
        const gpio_config_t cfg = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << BOARD_PIN_POWER,
        };
        ESP_RETURN_ON_ERROR(gpio_config(&cfg), "board", "power pin");
        configured = true;
    }
    return gpio_set_level(BOARD_PIN_POWER, on ? 1 : 0);
}
