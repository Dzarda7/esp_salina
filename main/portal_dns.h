/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Catch-all DNS used only while the setup portal is up.
 */
#pragma once

#include <stdint.h>

#include "esp_err.h"

/** @brief Answer every query with @p addr, a network-order IPv4 address. */
esp_err_t portal_dns_start(uint32_t addr);
void portal_dns_stop(void);
