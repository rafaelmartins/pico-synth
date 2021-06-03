#pragma once

#include <stdint.h>
#include <hardware/pio.h>
#include <pico/mutex.h>

typedef uint16_t (*mcp4822_sample_cb_t)(void *data);

typedef enum {
    MCP4822_DAC_A,
    MCP4822_DAC_B,
} mcp4822_dac_t;

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
    uint16_t data[2];
    bool resend;
} mcp4822_t;

void mcp4822_init(mcp4822_t *m);
bool mcp4822_update(mcp4822_t *m);
void mcp4822_set_cb(mcp4822_t *m, mcp4822_dac_t dac, mcp4822_sample_cb_t cb);
void mcp4822_set_cb_data(mcp4822_t *m, mcp4822_dac_t dac, void *cb_data);
