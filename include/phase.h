#pragma once

#include <stdint.h>

typedef struct {
    uint16_t pint;
    uint16_t pfrac;
} phase_t;

void phase_reset(phase_t *p);
uint16_t phase_step(phase_t *p, const phase_t *s);
