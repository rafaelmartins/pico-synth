/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings-channel.h"
#include "screen-settings-channel-midi.h"


static bool
title(ps_tui_t *tui, char *buf, size_t buflen)
{
    synth_t *synth = tui->ctx_data;
    snprintf(buf, buflen, "MIDI Channel (%d)", synth->_current_voice + 1);
    return true;
}


static uint8_t
getter(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    return synth->settings.voices[synth->_current_voice].midi_channel;
}


static void
setter(ps_tui_t *tui, uint8_t b)
{
    synth_t *synth = tui->ctx_data;
    synth->settings.voices[synth->_current_voice].midi_channel = b;
}


static const ps_tui_screen_select_byte_t select_byte = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_FUNC,
        .func = title,
    },
    .getter_func = getter,
    .setter_func = setter,
    .action = {
        .type = PS_TUI_SCREEN_ACTION_NEXT,
        .next = &screen_settings_channel,
    },
    .max = 0xf,
};

const ps_tui_screen_t screen_settings_channel_midi = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte,
};
