#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/time.h>
#include <pico-synth/engine.h>
#include <pico-synth/midi.h>
#include <pico-synth/tui.h>
#include "screens.h"
#include "settings.h"
#include "channel.h"

static settings_ctx_t settings;

static ps_tui_t tui = {
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

    .ctx_data = &settings,
};

static ps_engine_t engine = {
    .dac = {
        .pio = pio0,
        .state_machine = 0,
        .basepin = 10,
        .with_ldac = true,
    },
};

static ps_midi_t midi = {
    .uart = uart1,
    .uart_rx = 5,
    .with_usb = true,
};

static channel_t channels[] = {
    {
        .voices = {
            {
                .midi_channel = 0,
            },
            {
                .midi_channel = 0,
            },
            {
                .midi_channel = 0,
            },
        },
    },
    {
        .voices = {
            {
                .midi_channel = 1,
            },
            {
                .midi_channel = 1,
            },
            {
                .midi_channel = 1,
            },
        },
    },
};


static void
main1(void)
{
    channel_init(&channels[0], &engine.channel[0]);
    channel_init(&channels[1], &engine.channel[1]);

    hard_assert(ps_engine_init(&engine) == PICO_OK);

    ps_engine_module_oscillator_set_waveform(&channels[0].voices[0].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&channels[0].voices[1].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&channels[0].voices[2].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&channels[1].voices[0].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&channels[1].voices[1].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);
    ps_engine_module_oscillator_set_waveform(&channels[1].voices[2].oscillator, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE);

    ps_engine_module_mixer_set_master_gain(&channels[0].mixer, 127/3);
    ps_engine_module_mixer_set_master_gain(&channels[1].mixer, 127/3);

    while (1) {
        hard_assert(ps_engine_task(&engine) == PICO_OK);
    }
}


void
ps_midi_channel_cb(ps_midi_command_type_t cmd, uint8_t channel, uint8_t *data, uint8_t data_len)
{
    switch (cmd) {
    case PS_MIDI_COMMAND_NOTE_ON:
        if (data[1] != 0) {
            channel_set_note(&channels[0], channel, data[0], data[1]) ||
                channel_set_note(&channels[1], channel, data[0], data[1]);
            break;
        }

    case PS_MIDI_COMMAND_NOTE_OFF:
        channel_unset_note(&channels[0], channel, data[0]) ||
            channel_unset_note(&channels[1], channel, data[0]);
        break;
    }
}


int
main(void)
{
    set_sys_clock_khz(133000, true);
    stdio_uart_init_full(uart0, 115200, 16, 17);

    ps_midi_init(&midi);
    hard_assert(ps_tui_init(&tui) == PICO_OK);

    multicore_launch_core1(main1);

    ps_tui_screen_load(&tui, &screen_splash);
    sleep_ms(2000);
    ps_tui_screen_load(&tui, &screen_main);


    while (1) {
        ps_midi_task(&midi);

        hard_assert(ps_tui_task(&tui) == PICO_OK);
    }

    return 0;
}
