/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico-synth/midi.h>
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
        case PS_MIDI_MESSAGE_TYPE_CHANNEL_CONTROL_CHANGE:
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
