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

        .ctx_data = &synth,
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

        .ctx_data = &synth,
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
