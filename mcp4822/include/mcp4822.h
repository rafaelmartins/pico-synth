#pragma once

#include <hardware/pio.h>

typedef struct {
    PIO pio;
    uint sm;
    uint basepin;
} mcp4822_t;

void mcp4822_init(mcp4822_t *m);
bool mcp4822_set(mcp4822_t *m, uint16_t data_a, uint16_t data_b);
bool mcp4822_unset(mcp4822_t *m);
void mcp4822_set_clkdiv(mcp4822_t *m, uint16_t div_int, uint8_t div_frac);
