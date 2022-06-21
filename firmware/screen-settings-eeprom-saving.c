/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "screen-settings-eeprom-saving.h"

static const ps_tui_screen_lines_t lines = {
    .line3 = {"Saving EEPROM data", PS_TUI_OLED_HALIGN_CENTER},
};

const ps_tui_screen_t screen_settings_eeprom_saving = {
    .type = PS_TUI_SCREEN_LINES,
    .lines = &lines,
};
