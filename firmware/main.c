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
        .uart = uart1,
        .uart_rx = 5,
        .with_usb = true,
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
ps_midi_channel_cb(ps_midi_command_type_t cmd, uint8_t channel, uint8_t *data, uint8_t data_len)
{
    switch (cmd) {
    case PS_MIDI_COMMAND_NOTE_ON:
        if (data[1] != 0) {
            channel_set_note(&synth, channel, data[0], data[1]);
            break;
        }

    case PS_MIDI_COMMAND_NOTE_OFF:
        channel_unset_note(&synth, channel , data[0]);
        break;

    case PS_MIDI_COMMAND_POLYPHONIC_PRESSURE:
    case PS_MIDI_COMMAND_CONTROL_CHANGE:
    case PS_MIDI_COMMAND_PROGRAM_CHANGE:
    case PS_MIDI_COMMAND_CHANNEL_PRESSURE:
    case PS_MIDI_COMMAND_PITCH_BEND:
    case PS_MIDI_COMMAND_SYSTEM:
    case PS_MIDI_COMMAND_UNSET:
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
