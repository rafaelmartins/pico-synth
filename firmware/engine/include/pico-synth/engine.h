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

typedef enum {
    PS_ENGINE_MODULE_TYPE_SOURCE = 1,
    PS_ENGINE_MODULE_TYPE_FILTER,
    PS_ENGINE_MODULE_TYPE_SINK,
} ps_engine_module_type_t;

typedef void (*ps_engine_module_init_func_t)(void *ctx);
typedef int16_t (*ps_engine_module_sample_func_t)(int16_t in, ps_engine_phase_t *p, void *ctx);

typedef struct {
    ps_engine_module_type_t type;
    ps_engine_module_init_func_t init_func;
    ps_engine_module_sample_func_t sample_func;
} ps_engine_module_t;

typedef struct ps_engine_module_ctx {
    const ps_engine_module_t *mod;
    struct ps_engine_module_ctx *next;
    void *data;
} ps_engine_module_ctx_t;

typedef struct {
    struct {
        PIO pio;
        uint state_machine;
        uint basepin;
        bool with_ldac;
    } dac;

    struct {
        ps_engine_module_ctx_t *root;
        ps_engine_phase_t phase;
    } channel[2];
} ps_engine_t;

const ps_engine_note_t* ps_engine_get_note(uint8_t note);
const char* ps_engine_get_note_name(uint8_t note);

int ps_engine_init(ps_engine_t *e);
int ps_engine_task(ps_engine_t *e);
