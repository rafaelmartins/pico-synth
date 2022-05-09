#include <pico-synth/engine/module-amplifier.h>
#include <pico-synth/engine/module-mixer.h>
#include <pico-synth/engine/module-oscillator.h>
#include "channel.h"


void
channel_init(channel_t *chan, ps_engine_channel_t *engine_chan)
{
    hard_assert(chan);
    hard_assert(engine_chan);

    for (uint8_t i = 0; i < num_voices; i++) {
        channel_voice_t *voice = &chan->voices[i];

        voice->oscillator_src.mod = &ps_engine_module_oscillator;
        voice->oscillator_src.data = &voice->oscillator;
        voice->amplifier_fltr.mod = &ps_engine_module_amplifier;
        voice->amplifier_fltr.data = &voice->amplifier;

        voice->voice.source = &voice->oscillator_src;
        voice->voice.filters = &voice->amplifier_fltr;

        if (i > 0)
            chan->voices[i-1].voice.next = &voice->voice;
    }

    chan->mixer_sink.mod = &ps_engine_module_mixer;
    chan->mixer_sink.data = &chan->mixer;

    engine_chan->sink = &chan->mixer_sink;
    engine_chan->voices = &chan->voices[0].voice;
}


bool
channel_set_note(channel_t *chan, uint8_t midi_ch, uint8_t note, uint8_t velocity)
{
    hard_assert(chan);

    channel_voice_t *v = NULL;
    for (uint8_t i = 0; i < num_voices; i++) {
        if (chan->voices[i].midi_channel == midi_ch && !chan->voices[i].running) {
            v = &chan->voices[i];
            break;
        }
    }
    if (v == NULL)
        return false;

    ps_engine_module_oscillator_set_note(&v->oscillator, note);
    ps_engine_module_amplifier_set_gate(&v->amplifier, velocity);
    v->running = true;
    v->note = note;

    return true;
}


bool
channel_unset_note(channel_t *chan, uint8_t midi_ch, uint8_t note)
{
    hard_assert(chan);

    channel_voice_t *v = NULL;
    for (uint8_t i = 0; i < num_voices; i++) {
        if (chan->voices[i].midi_channel == midi_ch && chan->voices[i].running && chan->voices[i].note == note) {
            v = &chan->voices[i];
            break;
        }
    }
    if (v == NULL)
        return false;

    ps_engine_module_amplifier_unset_gate(&v->amplifier);
    v->running = false;

    return true;
}
