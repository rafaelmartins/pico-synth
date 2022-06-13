/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico/stdlib.h>
#include <pico/time.h>
#include "screens.h"
#include "synth.h"
#include "synth-data.h"


void
synth_init(synth_t *s)
{
    hard_assert(s);

    set_sys_clock_khz(cpu_frequency, true);

    if (s->stdio.uart != NULL)
        stdio_uart_init_full(s->stdio.uart, s->stdio.uart_baud_rate,
                             s->stdio.uart_tx, s->stdio.uart_rx);

    ps_midi_init(&s->midi);
    hard_assert(ps_tui_init(&s->tui) == PICO_OK);

    for (uint8_t i = 0; i < 2; i++) {
        if (s->channels[i].with_led) {
            gpio_init(s->channels[i].led);
            gpio_set_dir(s->channels[i].led, true);
        }

        s->channels[i].note = 0xff;
    }
}


void
synth_core0(synth_t *s)
{
    hard_assert(s);

    ps_tui_screen_load(&s->tui, &screen_splash);
    sleep_ms(2000);
    ps_tui_screen_load(&s->tui, &screen_main);

    while (1) {
        ps_midi_task(&s->midi);
        hard_assert(ps_tui_task(&s->tui) == PICO_OK);
    }
}


void
synth_core1(synth_t *s)
{
    hard_assert(s);

    for (uint8_t i = 0; i < 2; i++) {
        s->channels[i].oscillator_src.mod = &ps_engine_module_oscillator;
        s->channels[i].oscillator_src.data = &s->channels[i].oscillator;
        s->channels[i].amplifier_fltr.mod = &ps_engine_module_amplifier;
        s->channels[i].amplifier_fltr.data = &s->channels[i].amplifier;
        s->channels[i].adsr_fltr.mod = &ps_engine_module_adsr;
        s->channels[i].adsr_fltr.data = &s->channels[i].adsr;

        s->channels[i].voice.source = &s->channels[i].oscillator_src;
        s->channels[i].voice.filters = &s->channels[i].amplifier_fltr;
        s->channels[i].amplifier_fltr.next = &s->channels[i].adsr_fltr;

        s->engine.channels[i].voices = &s->channels[i].voice;
    }

    hard_assert(ps_engine_init(&s->engine) == PICO_OK);

    ps_engine_module_oscillator_set_waveform(&s->channels[0].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE);
    ps_engine_module_oscillator_set_waveform(&s->channels[1].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE);

    ps_engine_module_adsr_set_attack(&s->channels[0].adsr, 20);
    ps_engine_module_adsr_set_decay(&s->channels[0].adsr, 20);
    ps_engine_module_adsr_set_release(&s->channels[0].adsr, 20);
    ps_engine_module_adsr_set_sustain(&s->channels[0].adsr, 0x7f);

    ps_engine_module_adsr_set_attack(&s->channels[1].adsr, 20);
    ps_engine_module_adsr_set_decay(&s->channels[1].adsr, 20);
    ps_engine_module_adsr_set_release(&s->channels[1].adsr, 20);
    ps_engine_module_adsr_set_sustain(&s->channels[1].adsr, 0x7f);

    while (1)
        hard_assert(ps_engine_task(&s->engine) == PICO_OK);
}


void
channel_set_note(synth_t *s, uint8_t midi_ch, uint8_t note, uint8_t velocity)
{
    if (s == NULL)
        return;

    synth_channel_t *c = NULL;
    for (uint8_t i = 0; i < 2; i++) {
        if (s->channels[i].midi_channel == midi_ch && !s->channels[i].running) {
            c = &s->channels[i];
            break;
        }
    }
    if (c == NULL)
        return;

    ps_engine_module_oscillator_set_note(&c->oscillator, note);
    ps_engine_module_amplifier_set_gate(&c->amplifier, velocity);
    ps_engine_module_adsr_set_gate(&c->adsr);

    if (c->with_led)
        gpio_put(c->led, true);

    c->running = true;
    c->note = note;
}


void
channel_unset_note(synth_t *s, uint8_t midi_ch, uint8_t note)
{
    if (s == NULL)
        return;

    synth_channel_t *c = NULL;
    for (uint8_t i = 0; i < 2; i++) {
        if (s->channels[i].midi_channel == midi_ch && s->channels[i].running && s->channels[i].note == note) {
            c = &s->channels[i];
            break;
        }
    }
    if (c == NULL)
        return;

    ps_engine_module_adsr_unset_gate(&c->adsr);

    if (c->with_led)
        gpio_put(c->led, false);

    c->running = false;
    c->note = 0xff;
}
