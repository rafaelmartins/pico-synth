/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <hardware/uart.h>
#include <pico-synth/engine.h>
#include <pico-synth/engine/module-adsr.h>
#include <pico-synth/engine/module-amplifier.h>
#include <pico-synth/engine/module-oscillator.h>
#include <pico-synth/midi.h>
#include <pico-synth/tui.h>
#include "eeprom-types.h"

typedef struct {
    ps_engine_module_oscillator_ctx_t oscillator;
    ps_engine_module_source_ctx_t oscillator_src;

    ps_engine_module_amplifier_ctx_t amplifier;
    ps_engine_module_filter_ctx_t amplifier_fltr;

    ps_engine_module_adsr_ctx_t adsr;
    ps_engine_module_filter_ctx_t adsr_fltr;

    ps_engine_voice_t voice;

    uint led;
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

    eeprom_settings_t settings;
    synth_channel_t channels[2];

    // private
    uint8_t _current_voice;
    uint8_t _current_preset;
    bool _preset_from_channel;
    const ps_tui_screen_t *_midi_notifications_screen;
    uint8_t _midi_notifications_preset;
} synth_t;

void synth_init(synth_t *s);
void synth_core0(synth_t *s);
void synth_core1(synth_t *s);
void synth_set_note(synth_t *s, uint8_t midi_ch, uint8_t note, uint8_t velocity);
void synth_unset_note(synth_t *s, uint8_t midi_ch, uint8_t note);
uint8_t synth_preset_get_from_midi_channel(synth_t *synth, uint8_t midi_ch);
void synth_preset_set_waveform(synth_t *synth, uint8_t preset, ps_engine_module_oscillator_waveform_t wf);
void synth_preset_set_adsr_attack(synth_t *synth, uint8_t preset, uint8_t attack);
void synth_preset_set_adsr_decay(synth_t *synth, uint8_t preset, uint8_t decay);
void synth_preset_set_adsr_sustain(synth_t *synth, uint8_t preset, uint8_t sustain);
void synth_preset_set_adsr_release(synth_t *synth, uint8_t preset, uint8_t release);
