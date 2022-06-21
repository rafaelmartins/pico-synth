/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico/stdlib.h>
#include <pico/time.h>
#include "eeprom-types.h"
#include "screen-main.h"
#include "screen-splash.h"
#include "synth.h"
#include "synth-data.h"


static inline int
settings_init(ps_tui_t *t, eeprom_settings_t *s)
{
    hard_assert(t);
    hard_assert(s);

    int rv = ps_tui_eeprom_read(t, &s->version, sizeof(s->version));
    if (rv != PICO_OK)
        return rv;

    // if version value is not initialized we assume the whole eeprom is not initialized
    if (s->version == 0xffff) {
        s->version = EEPROM_SETTINGS_VERSION;

        for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
            s->voices[i].midi_channel = i;

        for (uint8_t i = 0; i < EEPROM_PRESET_COUNT; i++)
            s->presets[i].adsr.sustain = 0xff;

        return ps_tui_eeprom_write(t, s, sizeof(eeprom_settings_t));
    }

    return ps_tui_eeprom_read(t, s, sizeof(eeprom_settings_t));
}


static inline ps_engine_voice_t*
channel_init(synth_channel_t *c)
{
    hard_assert(c);

    if (c->with_led) {
        gpio_init(c->led);
        gpio_set_dir(c->led, true);
    }

    c->note = 0xff;

    c->oscillator_src.mod = &ps_engine_module_oscillator;
    c->oscillator_src.data = &c->oscillator;
    c->amplifier_fltr.mod = &ps_engine_module_amplifier;
    c->amplifier_fltr.data = &c->amplifier;
    c->adsr_fltr.mod = &ps_engine_module_adsr;
    c->adsr_fltr.data = &c->adsr;

    c->voice.source = &c->oscillator_src;
    c->voice.filters = &c->amplifier_fltr;
    c->amplifier_fltr.next = &c->adsr_fltr;

    return &c->voice;
}


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

    for (uint8_t i = 0; i < 2; i++)
        s->engine.channels[i].voices = channel_init(&s->channels[i]);

    ps_tui_screen_load(&s->tui, &screen_splash);
    hard_assert(PICO_OK == settings_init(&s->tui, &s->settings));
    ps_tui_screen_load(&s->tui, &screen_main);
}


void
synth_core0(synth_t *s)
{
    hard_assert(s);

    while (1) {
        ps_midi_task(&s->midi);
        hard_assert(ps_tui_task(&s->tui) == PICO_OK);
    }
}


static inline void
channel_apply_preset(synth_channel_t *c, eeprom_preset_t *p)
{
    if (p == NULL || c == NULL)
        return;

    ps_engine_module_oscillator_set_waveform(&c->oscillator, p->oscillator.waveform);
    ps_engine_module_adsr_set_attack(&c->adsr, p->adsr.attack);
    ps_engine_module_adsr_set_decay(&c->adsr, p->adsr.decay);
    ps_engine_module_adsr_set_sustain(&c->adsr, p->adsr.sustain);
    ps_engine_module_adsr_set_release(&c->adsr, p->adsr.release);
}


void
synth_core1(synth_t *s)
{
    hard_assert(s);
    hard_assert(ps_engine_init(&s->engine) == PICO_OK);

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        channel_apply_preset(&s->channels[i], &s->settings.presets[s->settings.voices[i].preset]);

    while (1)
        hard_assert(ps_engine_task(&s->engine) == PICO_OK);
}


void
synth_set_note(synth_t *s, uint8_t midi_ch, uint8_t note, uint8_t velocity)
{
    if (s == NULL)
        return;

    synth_channel_t *c = NULL;
    for (uint8_t i = 0; i < 2; i++) {
        if (s->settings.voices[i].midi_channel == midi_ch && !s->channels[i].running) {
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
synth_unset_note(synth_t *s, uint8_t midi_ch, uint8_t note)
{
    if (s == NULL)
        return;

    synth_channel_t *c = NULL;
    for (uint8_t i = 0; i < 2; i++) {
        if (s->settings.voices[i].midi_channel == midi_ch && s->channels[i].running && s->channels[i].note == note) {
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


uint8_t
synth_preset_get_from_midi_channel(synth_t *synth, uint8_t midi_ch)
{
    if (synth == NULL)
        return 0xff;

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].midi_channel == midi_ch)
            return synth->settings.voices[i].preset;

    return 0xff;
}


void
synth_preset_set_waveform(synth_t *synth, uint8_t preset, ps_engine_module_oscillator_waveform_t wf)
{
    if (synth == NULL || preset >= EEPROM_PRESET_COUNT)
        return;

    synth->settings.presets[preset].oscillator.waveform = wf;
    ps_tui_eeprom_write_lazy(&synth->tui, &synth->settings.presets[preset].oscillator.waveform,
                             sizeof(synth->settings.presets[preset].oscillator.waveform));

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].preset == preset)
            ps_engine_module_oscillator_set_waveform(&synth->channels[i].oscillator, wf);
}


void
synth_preset_set_adsr_attack(synth_t *synth, uint8_t preset, uint8_t attack)
{
    if (synth == NULL || preset >= EEPROM_PRESET_COUNT)
        return;

    synth->settings.presets[preset].adsr.attack = attack;
    ps_tui_eeprom_write_lazy(&synth->tui, &synth->settings.presets[preset].adsr.attack,
                             sizeof(synth->settings.presets[preset].adsr.attack));

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].preset == preset)
            ps_engine_module_adsr_set_attack(&synth->channels[i].adsr, attack);
}


void
synth_preset_set_adsr_decay(synth_t *synth, uint8_t preset, uint8_t decay)
{
    if (synth == NULL || preset >= EEPROM_PRESET_COUNT)
        return;

    synth->settings.presets[preset].adsr.decay = decay;
    ps_tui_eeprom_write_lazy(&synth->tui, &synth->settings.presets[preset].adsr.decay,
                             sizeof(synth->settings.presets[preset].adsr.decay));

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].preset == preset)
            ps_engine_module_adsr_set_decay(&synth->channels[i].adsr, decay);
}


void
synth_preset_set_adsr_sustain(synth_t *synth, uint8_t preset, uint8_t sustain)
{
    if (synth == NULL || preset >= EEPROM_PRESET_COUNT)
        return;

    synth->settings.presets[preset].adsr.sustain = sustain;
    ps_tui_eeprom_write_lazy(&synth->tui, &synth->settings.presets[preset].adsr.sustain,
                             sizeof(synth->settings.presets[preset].adsr.sustain));

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].preset == preset)
            ps_engine_module_adsr_set_sustain(&synth->channels[i].adsr, sustain);
}


void
synth_preset_set_adsr_release(synth_t *synth, uint8_t preset, uint8_t release)
{
    if (synth == NULL || preset >= EEPROM_PRESET_COUNT)
        return;

    synth->settings.presets[preset].adsr.release = release;
    ps_tui_eeprom_write_lazy(&synth->tui, &synth->settings.presets[preset].adsr.release,
                             sizeof(synth->settings.presets[preset].adsr.release));

    for (uint8_t i = 0; i < EEPROM_VOICE_COUNT; i++)
        if (synth->settings.voices[i].preset == preset)
            ps_engine_module_adsr_set_release(&synth->channels[i].adsr, release);
}
