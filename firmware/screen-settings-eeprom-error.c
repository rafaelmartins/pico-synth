/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "screen-settings.h"
#include "screen-settings-eeprom-error.h"

static const ps_tui_screen_lines_t lines = {
    .line3 = {"EEPROM operation", PS_TUI_OLED_HALIGN_CENTER},
    .line4 = {"failed :(", PS_TUI_OLED_HALIGN_CENTER},
    .action = {
        .type = PS_TUI_SCREEN_ACTION_NEXT,
        .next = &screen_settings,
    },
};

const ps_tui_screen_t screen_settings_eeprom_error = {
    .type = PS_TUI_SCREEN_LINES,
    .lines = &lines,
};
