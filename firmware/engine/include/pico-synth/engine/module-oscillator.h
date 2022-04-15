#pragma once

#include <pico/mutex.h>
#include <stdbool.h>
#include <pico-synth/engine.h>

typedef enum {
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE,
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE,
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_TRIANGLE,
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_LEFT_SAW,
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_RIGHT_SAW,
    PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM__LAST,
} ps_engine_module_oscillator_waveform_t;

typedef struct {
    // private
    mutex_t _mtx;
    ps_engine_module_oscillator_waveform_t _wf;
    const ps_engine_note_t *_note;
    bool _sync;
} ps_engine_module_oscillator_ctx_t;

extern ps_engine_module_source_t ps_engine_module_oscillator;

void ps_engine_module_oscillator_set_waveform(ps_engine_module_oscillator_ctx_t *ctx,
                                              ps_engine_module_oscillator_waveform_t wf);
void ps_engine_module_oscillator_set_note(ps_engine_module_oscillator_ctx_t *ctx,
                                          uint8_t note);
void ps_engine_module_oscillator_unset_note(ps_engine_module_oscillator_ctx_t *ctx,
                                            uint8_t note);
