#pragma once

#include <stdint.h>
#include <phase.h>

typedef struct {
    uint8_t id;
    const char *name;
    phase_t step;
} note_t;

const note_t* note_get(uint8_t note);
