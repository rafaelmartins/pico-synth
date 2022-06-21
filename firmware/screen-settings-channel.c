/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings-channels.h"
#include "screen-settings-channel-midi.h"
#include "screen-settings-channel-preset.h"
#include "screen-settings-preset.h"
#include "screen-settings-channel.h"


static bool
title(ps_tui_t *tui, char *buf, size_t buflen)
{
    synth_t *synth = tui->ctx_data;
    snprintf(buf, buflen, "Channel %d", synth->_current_voice + 1);
    return true;
}


static void
edit_preset(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    synth->_preset_from_channel = true;
    synth->_current_preset = synth->settings.voices[synth->_current_voice].preset;
    ps_tui_screen_load(tui, &screen_settings_preset);
}


static const ps_tui_screen_menu_t menu = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_FUNC,
        .func = title,
    },
    .items = {
        {
            .content = "MIDI Channel",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_channel_midi,
            }
        },
        {
            .content = "Preset",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_channel_preset,
            },
        },
        {
            .content = "-- Edit Preset",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = edit_preset,
            },
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_channels,
            },
        },
    },
    .num_items = 4,
};

const ps_tui_screen_t screen_settings_channel = {
    .type = PS_TUI_SCREEN_MENU,
    .menu = &menu,
};
