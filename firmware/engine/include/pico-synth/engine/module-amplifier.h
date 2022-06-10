/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <pico/mutex.h>
#include <pico-synth/engine.h>

typedef enum {
    PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF,
    PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON,
    PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON,
    PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF,
    PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN,
} ps_engine_module_amplifier_gate_state_t;

typedef struct {
    // private
    mutex_t _mtx;
    uint8_t _gain;      // 0 = 0, 127 = 1, 255 = 2
    uint8_t _gain_next; // 0 = 0, 127 = 1, 255 = 2
    uint16_t _pint;
    ps_engine_module_amplifier_gate_state_t _gate;
} ps_engine_module_amplifier_ctx_t;

extern const ps_engine_module_filter_t ps_engine_module_amplifier;

void ps_engine_module_amplifier_set_gate(ps_engine_module_amplifier_ctx_t *ctx, uint8_t gain);
void ps_engine_module_amplifier_unset_gate(ps_engine_module_amplifier_ctx_t *ctx);
