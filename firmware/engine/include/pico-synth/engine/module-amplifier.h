#pragma once

#include <pico/mutex.h>
#include <stdbool.h>
#include <pico-synth/engine.h>

typedef struct {
    // private
    mutex_t _mtx;
    uint8_t _gain; // 0 = 0, 127 = 1, 255 = 2
    bool _gate;
} ps_engine_module_amplifier_ctx_t;

extern const ps_engine_module_filter_t ps_engine_module_amplifier;

void ps_engine_module_amplifier_set_gate(ps_engine_module_amplifier_ctx_t *ctx, uint8_t gain);
void ps_engine_module_amplifier_unset_gate(ps_engine_module_amplifier_ctx_t *ctx);
