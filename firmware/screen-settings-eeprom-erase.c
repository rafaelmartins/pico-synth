/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hardware/watchdog.h>
#include "screen-settings.h"
#include "screen-settings-eeprom-error.h"
#include "screen-settings-eeprom-updating.h"


static void
erase_settings(ps_tui_t *tui)
{
    ps_tui_screen_load(tui, &screen_settings_eeprom_updating);
    if (PICO_OK != ps_tui_eeprom_erase(tui)) {
        ps_tui_screen_load(tui, &screen_settings_eeprom_error);
        sleep_ms(5000);
    }
    watchdog_reboot(0, 0, 0);
}


static const ps_tui_screen_menu_t menu = {
    .title = "Erase Settings?",
    .items = {
        {
            .content = "No",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .next = &screen_settings,
            },
        },
        {
            .content = "Yes",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_FUNC,
                .func = erase_settings,
            }
        },
    },
    .num_items = 2,
};

const ps_tui_screen_t screen_settings_eeprom_erase = {
    .type = PS_TUI_SCREEN_MENU,
    .menu = &menu,
};
