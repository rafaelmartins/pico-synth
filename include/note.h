#pragma once

#include <stdint.h>

typedef struct {
    uint8_t id;
    const char *name;
    uint8_t name_len;
    uint16_t step;
    uint16_t end;
    uint16_t div_int;
    uint8_t div_frac;
} note_t;

const note_t* note_get(uint8_t note);
