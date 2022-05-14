#pragma once

#include <hardware/uart.h>
#include <pico-synth/engine.h>
#include <pico-synth/engine/module-amplifier.h>
#include <pico-synth/engine/module-oscillator.h>
#include <pico-synth/midi.h>
#include <pico-synth/tui.h>
#include "settings.h"

typedef struct {
    ps_engine_module_oscillator_ctx_t oscillator;
    ps_engine_module_source_ctx_t oscillator_src;

    ps_engine_module_amplifier_ctx_t amplifier;
    ps_engine_module_filter_ctx_t amplifier_fltr;

    ps_engine_voice_t voice;

    uint led;
    uint8_t midi_channel;
    uint8_t note;
    bool running;
    bool with_led;
} synth_channel_t;

typedef struct {
    struct {
        uart_inst_t *uart;
        uint uart_baud_rate;
        uint uart_tx;
        uint uart_rx;
    } stdio;

    ps_engine_t engine;
    ps_midi_t midi;
    ps_tui_t tui;

    settings_ctx_t settings;

    synth_channel_t channels[2];
} synth_t;

void synth_core0(synth_t *s);
void synth_core1(synth_t *s);
void channel_set_note(synth_t *s, uint8_t midi_ch, uint8_t note, uint8_t velocity);
void channel_unset_note(synth_t *s, uint8_t midi_ch, uint8_t note);
