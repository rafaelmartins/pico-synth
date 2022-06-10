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
    PS_ENGINE_MODULE_ADSR_STATE_OFF,
    PS_ENGINE_MODULE_ADSR_STATE_ATTACK,
    PS_ENGINE_MODULE_ADSR_STATE_DECAY,
    PS_ENGINE_MODULE_ADSR_STATE_SUSTAIN,
    PS_ENGINE_MODULE_ADSR_STATE_RELEASE,
} ps_engine_module_adsr_state_t;

typedef struct {
    // private
    mutex_t _mtx;
    uint8_t _attack;  // 0 = 2ms, 255 = 20s
    uint8_t _decay;   // 0 = 2ms, 255 = 20s
    uint8_t _sustain; // 0 = 0%,  255 = 100%
    uint8_t _release; // 0 = 2ms, 255 = 20s
    uint16_t _level;
    uint16_t _range_start;
    uint16_t _range_end;
    ps_engine_phase_t _phase;
    ps_engine_module_adsr_state_t _state;
} ps_engine_module_adsr_ctx_t;

extern const ps_engine_module_filter_t ps_engine_module_adsr;

const char* ps_engine_module_adsr_get_time_description(uint8_t time);
void ps_engine_module_adsr_set_attack(ps_engine_module_adsr_ctx_t *ctx, uint8_t attack);
void ps_engine_module_adsr_set_decay(ps_engine_module_adsr_ctx_t *ctx, uint8_t decay);
void ps_engine_module_adsr_set_sustain(ps_engine_module_adsr_ctx_t *ctx, uint8_t sustain);
void ps_engine_module_adsr_set_release(ps_engine_module_adsr_ctx_t *ctx, uint8_t release);
void ps_engine_module_adsr_set_gate(ps_engine_module_adsr_ctx_t *ctx);
void ps_engine_module_adsr_unset_gate(ps_engine_module_adsr_ctx_t *ctx);
