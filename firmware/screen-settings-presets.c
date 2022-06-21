/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings-preset.h"
#include "screen-settings-presets.h"


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
    return synth->_current_preset;
}


static void
setter(ps_tui_t *tui, uint8_t b)
{
    synth_t *synth = tui->ctx_data;
    synth->_current_preset = b;
}


static void
action(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    synth->_preset_from_channel = false;
    ps_tui_screen_load(tui, &screen_settings_preset);
}


static const ps_tui_screen_select_byte_t select_byte = {
    .title = "Select Preset",
    .to_string_func = to_string,
    .getter_func = getter,
    .setter_func = setter,
    .action = {
        .type = PS_TUI_SCREEN_ACTION_FUNC,
        .func = action,
    },
    .max = EEPROM_PRESET_COUNT - 1,
};

const ps_tui_screen_t screen_settings_presets = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte,
};
