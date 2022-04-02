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

typedef struct {
    struct {
        PIO pio;
        uint state_machine;
        uint basepin;
    } dac;
} ps_engine_t;
