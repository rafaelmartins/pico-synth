#pragma once

#include <stdint.h>
#include <hardware/pio.h>
#include <pico/mutex.h>

#define mcp4822_min_int -0x0800
#define mcp4822_max_int  0x07ff
#define mcp4822_min_uint 0
#define mcp4822_max_uint 0x0fff
#define mcp4822_offset 0x0800

typedef enum {
    MCP4822_DAC_A,
    MCP4822_DAC_B,
} mcp4822_dac_t;

typedef int16_t (*mcp4822_sample_cb_t)(mcp4822_dac_t dac, void *data);

typedef struct {
    PIO pio;
    uint sm;
    uint basepin;
    float clkdiv;
    bool dual;

    // private fields
    mutex_t mtx;
    mcp4822_sample_cb_t cb[2];
    void *cb_data[2];
    int16_t data[2];
    bool resend;
} mcp4822_t;

void mcp4822_init(mcp4822_t *m);
bool mcp4822_update(mcp4822_t *m);
void mcp4822_set_cb(mcp4822_t *m, mcp4822_dac_t dac, mcp4822_sample_cb_t cb);
void mcp4822_set_cb_data(mcp4822_t *m, mcp4822_dac_t dac, void *cb_data);
