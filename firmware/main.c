#include <hardware/i2c.h>
#include <pico/multicore.h>
#include <tusb.h>
#include <synth.h>
#include <data/notes.h>

static synth_t synth = {
    .dac = {
        .pio = pio0,
        .sm = 0,
        .basepin = 6,
        .clkdiv = note_clkdiv,
        .dual = true,
    },

    .eeprom = {
        .i2c = i2c1,
        .ic = EEPROM_I2C_24LC512,
    },

    .encoder = {
        .button = 9,
        .a = 10,
        .b = 11,
        .button_debounce_us = 250000,
        .rotate_debounce_us = 2000,
    },

    .tui = {
        .i2c = i2c1,
        .oled_controller = OLED_TUI_SH1106,
    },

    .oscillators = {
        {
            .channel = MCP4822_DAC_A,
            .midi_channel = 0,
        },
        {
            .channel = MCP4822_DAC_A,
            .midi_channel = 1,
        },
        {
            .channel = MCP4822_DAC_A,
            .midi_channel = 2,
        },
        {
            .channel = MCP4822_DAC_B,
            .midi_channel = 3,
        },
        {
            .channel = MCP4822_DAC_B,
            .midi_channel = 4,
        },
        {
            .channel = MCP4822_DAC_B,
            .midi_channel = 5,
        },
    },

    .midi = {
        .uart = uart1,
    },
};


void
main() {
    set_sys_clock_khz(133000, true);

    stdio_init_all();
    tusb_init();

    i2c_init(i2c1, 400000);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);
    gpio_pull_up(2);
    gpio_pull_up(3);

    uart_init(uart1, 31250);
    gpio_set_function(5, GPIO_FUNC_UART);
    gpio_pull_up(5);

    hard_assert(synth_init(&synth) == PICO_OK);

    multicore_launch_core1(synth_core1);

    while (1) {
        tud_task();
        synth_task();
    }
}
