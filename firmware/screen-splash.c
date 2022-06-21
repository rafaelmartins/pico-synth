/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "screen-splash.h"

static const ps_tui_screen_lines_t lines = {
    .line1 = {"pico-synth", PS_TUI_OLED_HALIGN_CENTER},
    .line5 = {"(c) 2021-2022", PS_TUI_OLED_HALIGN_CENTER},
    .line6 = {"@rafaelmartins", PS_TUI_OLED_HALIGN_CENTER},
};

const ps_tui_screen_t screen_splash = {
    .type = PS_TUI_SCREEN_LINES,
    .lines = &lines,
};
