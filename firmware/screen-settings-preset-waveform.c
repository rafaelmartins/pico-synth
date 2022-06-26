/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "midi-notif.h"
#include "synth.h"
#include "screen-settings-preset.h"
#include "screen-settings-preset-waveform.h"


static void
to_string(ps_tui_t *tui, uint8_t b, char *buf, size_t buflen)
{
    switch ((ps_engine_module_oscillator_waveform_t) b) {
    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE:
        snprintf(buf, buflen, "Square");
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE:
        snprintf(buf, buflen, "Sine");
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_TRIANGLE:
        snprintf(buf, buflen, "Triangle");
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_LEFT_SAW:
        snprintf(buf, buflen, "Left Saw");
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_RIGHT_SAW:
        snprintf(buf, buflen, "Right Saw");
        break;

    default:
        break;
    }
}


static uint8_t
getter(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    return synth->settings.presets[synth->_current_preset].oscillator.waveform;
}


static void
setter(ps_tui_t *tui, uint8_t b)
{
    synth_t *synth = tui->ctx_data;
    synth_preset_set_waveform(synth, synth->_current_preset, b);
}


#define TITLE "Waveform"
#define _MAX PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM__LAST - 1

static const ps_tui_screen_select_byte_t select_byte = {
    .title = TITLE,
    .to_string_func = to_string,
    .getter_func = getter,
    .setter_func = setter,
    .action = {
        .type = PS_TUI_SCREEN_ACTION_NEXT,
        .next = &screen_settings_preset,
    },
    .max = _MAX,
};

const ps_tui_screen_t screen_settings_preset_waveform = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte,
};

static const ps_tui_screen_select_byte_t select_byte_notif = {
    .title = TITLE,
    .to_string_func = to_string,
    .getter_func = getter,
    .max = _MAX,
};

const ps_tui_screen_t screen_settings_preset_waveform_notif = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte_notif,
    .auto_action = {
        .delay_ms = MIDI_NOTIFICATIONS_DELAY_MS,
        .action = {
            .type = PS_TUI_SCREEN_ACTION_FUNC,
            .func = midi_notif_action,
        },
    }
};
