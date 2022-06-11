/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico/multicore.h>
#include "synth.h"

static synth_t synth = {
    .stdio = {
        .uart = uart0,
        .uart_baud_rate = 115200,
        .uart_tx = 16,
        .uart_rx = 17,
    },

    .engine.dac = {
        .pio = pio0,
        .state_machine = 0,
        .basepin = 10,
        .with_ldac = true,
    },

    .midi = {
        .uart = {
            .uart = uart1,
            .uart_rx = 5,
        },

        .usb = {
            .enable = true,
            .vid = 0x1d50,  // OpenMoko, Inc.
            .pid = 0x6179,  // pico-synth
            .version = 0x0100,
            .product = "pico-synth",
        },
    },

    .tui = {
        .i2c = {
            .i2c = i2c1,
            .baudrate = 400000,
            .basepin = 2,
        },

        .eeprom = {
            .ic = PS_TUI_EEPROM_24LC512,
        },

        .encoder = {
            .button = 6,
            .a = 8,
            .b = 7,
            .button_debounce_us = 250000,
            .rotate_debounce_us = 2000,
        },

        .oled = {
            .controller = PS_TUI_OLED_SH1106,
        },
    },

    .channels = {
        {
            .with_led = true,
            .led = 21,
            .midi_channel = 0,
        },
        {
            .with_led = true,
            .led = 20,
            .midi_channel = 0,
        },
    },
};


void
ps_midi_message_cb(const ps_midi_message_t *msg)
{
    switch (msg->group) {
    case PS_MIDI_MESSAGE_GROUP_CHANNEL:
        switch (msg->channel.type) {
        case PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_ON:
            if (msg->channel.data[1] != 0) {
                channel_set_note(&synth, msg->channel.channel, msg->channel.data[0], msg->channel.data[1]);
                break;
            }

        case PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_OFF:
            channel_unset_note(&synth, msg->channel.channel , msg->channel.data[0]);
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


static void
main1(void)
{
    synth_core1(&synth);
}


int
main(void)
{
    synth_init(&synth);
    multicore_launch_core1(main1);
    synth_core0(&synth);
    return 0;
}
