/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <pico/mutex.h>
#include <pico-synth/engine.h>

typedef struct {
    // private
    mutex_t _mtx;
    uint8_t _master_gain; // 0 = 0, 127 = 1, 255 = 2
} ps_engine_module_mixer_ctx_t;

extern const ps_engine_module_sink_t ps_engine_module_mixer;

void ps_engine_module_mixer_set_master_gain(ps_engine_module_mixer_ctx_t *ctx, uint8_t gain);
