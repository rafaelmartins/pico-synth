/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "synth.h"
#include "midi-notif.h"


void
midi_notif_action(ps_tui_t *tui)
{
    synth_t *synth = tui->ctx_data;
    synth->_current_preset = synth->_midi_notifications_preset;
    ps_tui_screen_load(tui, synth->_midi_notifications_screen);
}


void
midi_notif_load_screen(ps_tui_t *tui, const ps_tui_screen_t *screen, uint8_t preset)
{
    hard_assert(tui);
    if (screen == NULL)
        return;

    synth_t *synth = tui->ctx_data;
    if (synth->tui._current_screen->auto_action.action.type == 0) {
        synth->_midi_notifications_screen = synth->tui._current_screen;
        synth->_midi_notifications_preset = synth->_current_preset;
    }
    synth->_current_preset = preset;
    ps_tui_screen_load(&synth->tui, screen);
}
