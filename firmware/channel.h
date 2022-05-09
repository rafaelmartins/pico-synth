#pragma once

#include <pico-synth/engine/module-amplifier.h>
#include <pico-synth/engine/module-mixer.h>
#include <pico-synth/engine/module-oscillator.h>

#define num_voices 3

typedef struct {
    ps_engine_module_oscillator_ctx_t oscillator;
    ps_engine_module_source_ctx_t oscillator_src;

    ps_engine_module_amplifier_ctx_t amplifier;
    ps_engine_module_filter_ctx_t amplifier_fltr;

    ps_engine_voice_t voice;

    uint8_t midi_channel;
    bool running;
    uint8_t note;
} channel_voice_t;

typedef struct {
    ps_engine_module_mixer_ctx_t mixer;
    ps_engine_module_sink_ctx_t mixer_sink;

    channel_voice_t voices[num_voices];
} channel_t;

void channel_init(channel_t *chan, ps_engine_channel_t *engine_chan);
void channel_set_note(channel_t *chan, uint8_t midi_ch, uint8_t note, uint8_t velocity);
void channel_unset_note(channel_t *chan, uint8_t midi_ch, uint8_t note);
