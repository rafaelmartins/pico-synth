/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "synth.h"
#include "screen-settings.h"
#include "screen-main.h"


static void
render(ps_tui_t *tui)
{
    synth_t *s = tui->ctx_data;
    char buf[20];

    ps_tui_oled_clear(tui);
    ps_tui_oled_line(tui, 0, "pico-synth", PS_TUI_OLED_HALIGN_CENTER);

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++) {
        eeprom_preset_t *p = &s->settings.presets[s->settings.voices[i].preset];
        snprintf(buf, sizeof(buf), "%d: %s (%d)", i + 1,
                 p->name[0] == 0 ? "NO NAME" : p->name,
                 s->settings.voices[i].preset);
        ps_tui_oled_line(tui, (i + 1) * 3, buf, PS_TUI_OLED_HALIGN_LEFT);

        snprintf(buf, sizeof(buf), "   MIDI Channel: %d",
                 s->settings.voices[i].midi_channel);
        ps_tui_oled_line(tui, (i + 1) * 3 + 1, buf, PS_TUI_OLED_HALIGN_LEFT);
    }
}


static void
encoder(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    if (act != PS_TUI_ENCODER_ACTION_BUTTON)
        return;

    ps_tui_screen_load(tui, &screen_settings);
}


static const ps_tui_screen_func_t func = {
    .render_func = render,
    .encoder_func = encoder,
};

const ps_tui_screen_t screen_main = {
    .type = PS_TUI_SCREEN_FUNC,
    .func = &func,
};
