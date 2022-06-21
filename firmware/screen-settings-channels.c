/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "synth.h"
#include "screen-settings.h"
#include "screen-settings-channel.h"
#include "screen-settings-channels.h"


static void
select_1(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    synth->_current_voice = 0;
    ps_tui_screen_load(tui, &screen_settings_channel);
}


static void
select_2(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    synth->_current_voice = 1;
    ps_tui_screen_load(tui, &screen_settings_channel);
}


static const ps_tui_screen_menu_t menu = {
    .title = "Select Channel",
    .items = {
        {
            .content = "Channel 1",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = select_1,
            },
        },
        {
            .content = "Channel 2",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = select_2,
            }
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings,
            },
        },
    },
    .num_items = 3,
};

const ps_tui_screen_t screen_settings_channels = {
    .type = PS_TUI_SCREEN_MENU,
    .menu = &menu,
};
