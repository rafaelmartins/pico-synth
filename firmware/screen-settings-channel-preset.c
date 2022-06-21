/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings-channel.h"
#include "screen-settings-channel-preset.h"


static bool
title(ps_tui_t *tui, char *buf, size_t buflen)
{
    synth_t *synth = tui->ctx_data;
    snprintf(buf, buflen, "Preset (%d)", synth->_current_voice + 1);
    return true;
}


static void
to_string(ps_tui_t *tui, uint8_t b, char *buf, size_t buflen)
{
    synth_t *synth = tui->ctx_data;
    eeprom_preset_t *p = &synth->settings.presets[b];
    snprintf(buf, buflen, "%s (%d)", p->name[0] == 0 ? "NO NAME" : p->name, b);
}


static uint8_t
getter(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    return synth->settings.voices[synth->_current_voice].preset;
}


static void
setter(ps_tui_t *tui, uint8_t b)
{
    synth_t *synth = tui->ctx_data;
    synth->settings.voices[synth->_current_voice].preset = b;
    ps_tui_eeprom_write_lazy(tui, &synth->settings.voices[synth->_current_voice].preset,
                             sizeof(synth->settings.voices[synth->_current_voice].preset));
}


static const ps_tui_screen_select_byte_t select_byte = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_FUNC,
        .func = title,
    },
    .to_string_func = to_string,
    .getter_func = getter,
    .setter_func = setter,
    .action = {
        .type = PS_TUI_SCREEN_ACTION_NEXT,
        .next = &screen_settings_channel,
    },
    .max = EEPROM_PRESET_COUNT - 1,
};

const ps_tui_screen_t screen_settings_channel_preset = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte,
};
