/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings-preset.h"
#include "screen-settings-preset-adsr-release.h"


static void
to_string(ps_tui_t *tui, uint8_t b, char *buf, size_t buflen)
{
    snprintf(buf, buflen, "%s", ps_engine_module_adsr_get_time_description(b));
}


static uint8_t
getter(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    return synth->settings.presets[synth->_current_preset].adsr.release;
}


static void
setter(ps_tui_t *tui, uint8_t b)
{
    synth_t *synth = tui->ctx_data;
    synth_preset_set_adsr_release(synth, synth->_current_preset, b);
}


static const ps_tui_screen_select_byte_t select_byte = {
    .title = "ADSR Release",
    .to_string_func = to_string,
    .getter_func = getter,
    .setter_func = setter,
    .action = {
        .type = PS_TUI_SCREEN_ACTION_NEXT,
        .next = &screen_settings_preset,
    },
    .max = 0xff,
};

const ps_tui_screen_t screen_settings_preset_adsr_release = {
    .type = PS_TUI_SCREEN_SELECT_BYTE,
    .select_byte = &select_byte,
};
