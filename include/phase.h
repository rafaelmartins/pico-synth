#pragma once

#include <stdint.h>

#define PHASE_MAX 0x2000

typedef struct {
    uint16_t pint;
    uint16_t pfrac;
} phase_t;

void phase_reset(phase_t *p);
void phase_step(phase_t *p, const phase_t *s);
uint16_t phase_get_index(phase_t *p);
