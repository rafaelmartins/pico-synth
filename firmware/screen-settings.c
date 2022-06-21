/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "screen-main.h"
#include "screen-settings-channels.h"
#include "screen-settings-presets.h"
#include "screen-settings.h"

static const ps_tui_screen_menu_t menu = {
    .title = "Settings",
    .items = {
        {
            .content = "Channels",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_channels,
            },
        },
        {
            .content = "Presets",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_presets,
            }
        },
        {
            .content = "-- Save Settings",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = NULL,  // FIXME
            }
        },
        {
            .content = "!! Erase Settings",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = NULL,  // FIXME
            }
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_main,
            },
        },
    },
    .num_items = 5,
};

const ps_tui_screen_t screen_settings = {
    .type = PS_TUI_SCREEN_MENU,
    .menu = &menu,
};
