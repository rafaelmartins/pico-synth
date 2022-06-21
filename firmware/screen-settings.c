/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "screen-main.h"
#include "screen-settings-channels.h"
#include "screen-settings-eeprom-erase.h"
#include "screen-settings-eeprom-error.h"
#include "screen-settings-eeprom-updating.h"
#include "screen-settings-presets.h"
#include "screen-settings.h"


static void
save_settings(ps_tui_t *tui)
{
    ps_tui_screen_load(tui, &screen_settings_eeprom_updating);
    int rv = ps_tui_eeprom_sync(tui);
    ps_tui_screen_load(tui, rv == PICO_OK ? &screen_settings : &screen_settings_eeprom_error);
}


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
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = save_settings,
            }
        },
        {
            .content = "!! Erase Settings",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings_eeprom_erase,
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
