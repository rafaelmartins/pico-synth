/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <hardware/pio.h>

typedef union {
    uint32_t data;

    struct __attribute__((packed)) {
#ifdef __ARMEB__
        // I don't think that this is really needed for rp2040 or any upcoming
        // raspberry pi silicon, but why not!?
        uint16_t pint;
        uint16_t pfrac;
#else
        uint16_t pfrac;
        uint16_t pint;
#endif
    };
} ps_engine_phase_t;

typedef struct {
    uint8_t id;
    const char *name;
    ps_engine_phase_t step;
} ps_engine_note_t;

typedef void (*ps_engine_module_init_func_t)(void *ctx);

typedef int16_t (*ps_engine_module_source_sample_func_t)(ps_engine_phase_t *p, void *ctx);
typedef struct {
    ps_engine_module_init_func_t init_func;
    ps_engine_module_source_sample_func_t sample_func;
} ps_engine_module_source_t;

typedef int16_t (*ps_engine_module_filter_sample_func_t)(int16_t in, const ps_engine_phase_t *p, void *ctx);
typedef struct {
    ps_engine_module_init_func_t init_func;
    ps_engine_module_filter_sample_func_t sample_func;
} ps_engine_module_filter_t;

typedef int16_t (*ps_engine_module_sink_sample_func_t)(const int16_t *in, uint8_t in_len, void *ctx);
typedef struct {
    ps_engine_module_init_func_t init_func;
    ps_engine_module_sink_sample_func_t sample_func;
} ps_engine_module_sink_t;

typedef struct {
    const ps_engine_module_source_t *mod;
    void *data;
} ps_engine_module_source_ctx_t;

typedef struct ps_engine_module_filter_ctx {
    const ps_engine_module_filter_t *mod;
    struct ps_engine_module_filter_ctx *next;
    void *data;
} ps_engine_module_filter_ctx_t;

typedef struct {
    const ps_engine_module_sink_t *mod;
    void *data;
} ps_engine_module_sink_ctx_t;

typedef struct ps_engine_voice {
    ps_engine_phase_t phase;
    ps_engine_module_source_ctx_t *source;
    ps_engine_module_filter_ctx_t *filters;
    struct ps_engine_voice *next;
} ps_engine_voice_t;

typedef struct {
    ps_engine_module_sink_ctx_t *sink;
    ps_engine_voice_t *voices;

    // private
    uint8_t _num_voices;
} ps_engine_channel_t;

typedef struct {
    struct {
        PIO pio;
        uint state_machine;
        uint basepin;
        bool with_ldac;
    } dac;

    ps_engine_channel_t channels[2];
} ps_engine_t;

const ps_engine_note_t* ps_engine_get_note(uint8_t note);
const char* ps_engine_get_note_name(uint8_t note);

void ps_engine_phase_reset(ps_engine_phase_t *phase);
bool ps_engine_phase_step(ps_engine_phase_t *phase, const ps_engine_phase_t *step, uint16_t table_len);
uint16_t ps_engine_phase_next_pint(const ps_engine_phase_t *phase, uint16_t table_len);

int ps_engine_init(ps_engine_t *e);
int ps_engine_task(ps_engine_t *e);
