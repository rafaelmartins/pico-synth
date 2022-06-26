/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico-synth/midi.h>
#include "screen-settings-preset-adsr-attack.h"
#include "screen-settings-preset-adsr-decay.h"
#include "screen-settings-preset-adsr-release.h"
#include "screen-settings-preset-adsr-sustain.h"
#include "screen-settings-preset-waveform.h"
#include "midi-notif.h"
#include "synth.h"


void
ps_midi_message_cb(const ps_midi_message_t *msg, void *ctx_data)
{
    hard_assert(ctx_data);

    synth_t *synth = ctx_data;

    switch (msg->group) {
    case PS_MIDI_MESSAGE_GROUP_CHANNEL:
        switch (msg->channel.type) {
        case PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_ON:
            if (msg->channel.data[1] != 0) {
                synth_set_note(synth, msg->channel.channel, msg->channel.data[0], msg->channel.data[1]);
                break;
            }

        case PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_OFF:
            synth_unset_note(synth, msg->channel.channel , msg->channel.data[0]);
            break;

        case PS_MIDI_MESSAGE_TYPE_CHANNEL_POLYPHONIC_PRESSURE:
            break;

        case PS_MIDI_MESSAGE_TYPE_CHANNEL_CONTROL_CHANGE: {
            uint8_t p = synth_preset_get_from_midi_channel(synth, msg->channel.channel);

            switch (msg->channel.data[0]) {
            case 0x03: // waveform
                synth_preset_set_waveform(synth, p, msg->channel.data[1] / (0x80 / PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM__LAST));
                midi_notif_load_screen(&synth->tui, &screen_settings_preset_waveform_notif, p);
                break;

            case 0x48: // release
                synth_preset_set_adsr_release(synth, p, msg->channel.data[1] * 2);
                midi_notif_load_screen(&synth->tui, &screen_settings_preset_adsr_release_notif, p);
                break;

            case 0x49: // attack
                synth_preset_set_adsr_attack(synth, p, msg->channel.data[1] * 2);
                midi_notif_load_screen(&synth->tui, &screen_settings_preset_adsr_attack_notif, p);
                break;

            case 0x4B: // decay
                synth_preset_set_adsr_decay(synth, p, msg->channel.data[1] * 2);
                midi_notif_load_screen(&synth->tui, &screen_settings_preset_adsr_decay_notif, p);
                break;

            case 0x4F: // sustain
                synth_preset_set_adsr_sustain(synth, p, msg->channel.data[1] * 2);
                midi_notif_load_screen(&synth->tui, &screen_settings_preset_adsr_sustain_notif, p);
                break;
            }
        }

        case PS_MIDI_MESSAGE_TYPE_CHANNEL_PROGRAM_CHANGE:
        case PS_MIDI_MESSAGE_TYPE_CHANNEL_CHANNEL_PRESSURE:
        case PS_MIDI_MESSAGE_TYPE_CHANNEL_PITCH_BEND:
            break;
        }
        break;

    case PS_MIDI_MESSAGE_GROUP_SYSTEM:
    case PS_MIDI_MESSAGE_GROUP_SYSTEM_RT:
    case PS_MIDI_MESSAGE_GROUP_SYSEX:
        break;
    }
}
