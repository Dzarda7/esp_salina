/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * The one HTTPS GET both departure sources need.
 */
#pragma once

#include <stddef.h>

#include "esp_err.h"

/** @brief Largest reply either source is expected to produce. */
#define SALINA_RESPONSE_LIMIT 8192

/**
 * @brief GET a JSON document into a caller-provided buffer.
 *
 * @param[in]  host     Host name, TLS is always used
 * @param[in]  path     Path with query string
 * @param[in]  api_key  Sent as X-Access-Token, or NULL when none is needed
 * @param[out] buf      Receives the body, NUL terminated
 * @param[in]  buf_size Size of @p buf, including room for the terminator
 */
esp_err_t salina_http_get(const char *host, const char *path, const char *api_key, char *buf, size_t buf_size);

/** @brief Percent-encode everything that is not an unreserved URL character. */
void salina_url_encode(const char *in, char *out, size_t out_size);
