#pragma once

#include <hardware/pio.h>

typedef struct {
    uint16_t pint;
    uint16_t pfrac;
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
    ps_engine_phase_t phase;
    ps_engine_module_source_ctx_t *source;
    ps_engine_module_filter_ctx_t *filters;
} ps_engine_channel_t;

typedef struct {
    struct {
        PIO pio;
        uint state_machine;
        uint basepin;
        bool with_ldac;
    } dac;

    ps_engine_channel_t channel[2];
} ps_engine_t;

const ps_engine_note_t* ps_engine_get_note(uint8_t note);
const char* ps_engine_get_note_name(uint8_t note);

int ps_engine_init(ps_engine_t *e);
int ps_engine_task(ps_engine_t *e);
