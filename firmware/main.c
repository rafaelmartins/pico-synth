#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/time.h>
#include <pico-synth/engine.h>
#include <pico-synth/engine/module-amplifier.h>
#include <pico-synth/engine/module-oscillator.h>
#include <pico-synth/midi.h>
#include <pico-synth/tui.h>
#include "screens.h"
#include "settings.h"

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
        .button = 9,
        .a = 10,
        .b = 11,
        .button_debounce_us = 250000,
        .rotate_debounce_us = 2000,
    },

    .oled = {
        .controller = PS_TUI_OLED_SH1106,
    },

    .ctx_data = &settings,
};

static ps_engine_module_oscillator_ctx_t oscillator_ctx1;
static ps_engine_module_oscillator_ctx_t oscillator_ctx2;

static ps_engine_module_source_ctx_t oscillator_mod_ctx1 = {
    .mod = &ps_engine_module_oscillator,
    .data = &oscillator_ctx1,
};
static ps_engine_module_source_ctx_t oscillator_mod_ctx2 = {
    .mod = &ps_engine_module_oscillator,
    .data = &oscillator_ctx2,
};

static ps_engine_module_amplifier_ctx_t amplifier_ctx1;

static ps_engine_module_filter_ctx_t amplifier_mod_ctx1 = {
    .mod = &ps_engine_module_amplifier,
    .data = &amplifier_ctx1,
};


static ps_engine_t engine = {
    .dac = {
        .pio = pio0,
        .state_machine = 0,
        .basepin = 6,
    },

    .channel = {
        {
            .source = &oscillator_mod_ctx1,
            .filters = &amplifier_mod_ctx1,
        },
        {
            .source = &oscillator_mod_ctx2,
        },
    },
};


static ps_midi_t midi = {
    .uart = uart1,
    .uart_rx = 5,
    .with_usb = true,
};


static void
main1(void)
{
    hard_assert(ps_engine_init(&engine) == PICO_OK);

    ps_engine_module_oscillator_set_waveform(&oscillator_ctx1, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE);
    //ps_engine_module_oscillator_set_note(&oscillator_ctx1, 69);

    ps_engine_module_oscillator_set_waveform(&oscillator_ctx2, PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE);
    //ps_engine_module_oscillator_set_note(&oscillator_ctx2, 69);

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
            ps_engine_module_oscillator_set_note(&oscillator_ctx1, data[0]);
            ps_engine_module_amplifier_set_gate(&amplifier_ctx1, data[1]);
            break;
        }

    case PS_MIDI_COMMAND_NOTE_OFF:
        ps_engine_module_amplifier_unset_gate(&amplifier_ctx1);
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
