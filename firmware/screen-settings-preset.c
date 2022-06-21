/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings.h"
#include "screen-settings-channel.h"
#include "screen-settings-preset-adsr-attack.h"
#include "screen-settings-preset-adsr-decay.h"
#include "screen-settings-preset-adsr-release.h"
#include "screen-settings-preset-adsr-sustain.h"
#include "screen-settings-preset-waveform.h"
#include "screen-settings-preset.h"


static bool
title(ps_tui_t *tui, char *buf, size_t buflen)
{
    synth_t *synth = tui->ctx_data;
    eeprom_preset_t *p = &synth->settings.presets[synth->_current_preset];
    snprintf(buf, buflen, "Preset %s (%d)", p->name[0] == 0 ? "NO NAME" : p->name,
             synth->_current_preset);
    return true;
}


static void
back(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    if (synth->_preset_from_channel)
        ps_tui_screen_load(tui, &screen_settings_channel);
    else
        ps_tui_screen_load(tui, &screen_settings);
}


static const ps_tui_screen_menu_t menu = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_FUNC,
        .func = title,
    },
    .items = {
        {
            .content = "Name",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = NULL,  // FIXME: we have no way to edit strings so far
            },
        },
        {
            .content = "Osc. Waveform",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_preset_waveform,
            },
        },
        {
            .content = "ADSR Attack",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_preset_adsr_attack,
            }
        },
        {
            .content = "ADSR Decay",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_preset_adsr_decay,
            }
        },
        {
            .content = "ADSR Sustain",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_preset_adsr_sustain,
            }
        },
        {
            .content = "ADSR Release",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_preset_adsr_release,
            }
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = back,
            },
        },
    },
    .num_items = 7,
};

const ps_tui_screen_t screen_settings_preset = {
    .type = PS_TUI_SCREEN_MENU,
    .menu = &menu,
};
