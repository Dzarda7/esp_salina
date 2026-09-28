/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * The table of departure sources the firmware carries.
 */
#include "salina.h"
#include "salina_config.h"

extern const salina_source_t salina_source_idsjmk;
extern const salina_source_t salina_source_pid;

const salina_source_t *salina_source_get(salina_source_id_t id)
{
    static const salina_source_t *const sources[SALINA_SOURCE_COUNT] = {
        [SALINA_SOURCE_IDSJMK] = &salina_source_idsjmk,
        [SALINA_SOURCE_PID] = &salina_source_pid,
    };

    return (id >= 0 && id < SALINA_SOURCE_COUNT) ? sources[id] : NULL;
}
