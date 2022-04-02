#pragma once

typedef struct {
    uint16_t pint;
    uint16_t pfrac;
} phase_t;

typedef struct {
    uint8_t id;
    const char *name;
    phase_t step;
} note_t;
