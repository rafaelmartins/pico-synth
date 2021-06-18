#include <hardware/i2c.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <mcp4822.h>
#include <ssd1306.h>
#include <controller.h>
#include <oscillator.h>
#include <data/notes.h>

static mcp4822_t dac = {
    .pio = pio0,
    .sm = 0,
    .basepin = 6,
    .clkdiv = note_clkdiv,
    .dual = false,
};

static oscillator_t osc = {
    .id = 0,
};

static ssd1306_t display = {
    .i2c = i2c0,
};


void
main_core1() {
    while (true) {
        mcp4822_update(&dac);
    }
}


int
main() {
    set_sys_clock_khz(133000, true);

    stdio_init_all();
    tusb_init();

    i2c_init(i2c0, 400000);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);
    gpio_pull_up(2);
    gpio_pull_up(3);

    mcp4822_init(&dac);

    oscillator_init(&osc);
    mcp4822_set_cb_data(&dac, MCP4822_DAC_A, &osc);
    mcp4822_set_cb(&dac, MCP4822_DAC_A, oscillator_sample_callback);

    ssd1306_init(&display);

    controller_init(&display, &osc, NULL, NULL);

    multicore_launch_core1(main_core1);

    while (1) {
        tud_task();
        controller_midi_task();
    }
}
