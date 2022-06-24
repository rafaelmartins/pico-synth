/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <pico-synth/tui.h>

#define MIDI_NOTIFICATIONS_DELAY_MS 2000

void midi_notif_action(ps_tui_t *tui);
void midi_notif_load_screen(ps_tui_t *tui, const ps_tui_screen_t *screen, uint8_t preset);
