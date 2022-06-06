/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <pico-synth/tui.h>

#define FONT_WIDTH 5
#define FONT_HEIGHT 7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_CHARS_PER_LINE ((uint8_t) (SCREEN_WIDTH / (FONT_WIDTH + 1)))
#define RAM_PAGES          ((uint8_t) (SCREEN_HEIGHT / (FONT_HEIGHT + 1)))
// we abuse the fact that our rendered lines match nicely with the ram pages
#define MAX_LINES RAM_PAGES

int oled_init(ps_tui_t *tui);
int oled_render(ps_tui_t *tui);
