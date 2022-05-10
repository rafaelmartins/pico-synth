#include <pico/stdlib.h>
#include <pico/time.h>
#include "screens.h"
#include "synth.h"
#include "synth-data.h"


static void
channel_init(synth_channel_t *c, ps_engine_channel_t *ec)
{
    if (c == NULL || ec == NULL)
        return;

    c->oscillator_src.mod = &ps_engine_module_oscillator;
    c->oscillator_src.data = &c->oscillator;
    c->amplifier_fltr.mod = &ps_engine_module_amplifier;
    c->amplifier_fltr.data = &c->amplifier;

    c->voice.source = &c->oscillator_src;
    c->voice.filters = &c->amplifier_fltr;

    ec->voices = &c->voice;
}


void
synth_core0(synth_t *s)
{
    hard_assert(s);

    set_sys_clock_khz(cpu_frequency, true);

    if (s->stdio_uart.uart != NULL)
        stdio_uart_init_full(s->stdio_uart.uart, s->stdio_uart.baud_rate,
                             s->stdio_uart.tx, s->stdio_uart.rx);

    ps_midi_init(&s->midi);
    hard_assert(ps_tui_init(&s->tui) == PICO_OK);
    s->tui.ctx_data = &s->settings;

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

    channel_init(&s->channels[0], &s->engine.channels[0]);
    channel_init(&s->channels[1], &s->engine.channels[1]);

    hard_assert(ps_engine_init(&s->engine) == PICO_OK);

    ps_engine_module_oscillator_set_waveform(&s->channels[0].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&s->channels[1].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);

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

    ps_engine_module_amplifier_unset_gate(&c->amplifier);
    c->running = false;
}
