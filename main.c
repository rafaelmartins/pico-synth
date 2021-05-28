#include <hardware/i2c.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <mcp4822.h>
#include <ssd1306.h>
#include <controller.h>
#include <oscillator.h>

static mcp4822_t ddac1 = {
    .pio = pio0,
    .sm = 0,
    .basepin = 2,
};

static mcp4822_t ddac2 = {
    .pio = pio0,
    .sm = 1,
    .basepin = 5,
};

static mcp4822_t ddac3 = {
    .pio = pio0,
    .sm = 2,
    .basepin = 18,
};

static oscillator_t osc1 = {
    .id = 0,
    .dac = &ddac1,
};

static oscillator_t osc2 = {
    .id = 1,
    .dac = &ddac2,
};

static oscillator_t osc3 = {
    .id = 2,
    .dac = &ddac3,
};

static ssd1306_t display = {
    .i2c = i2c0,
};


void
main_core1() {
    while (true) {
        oscillator_update(&osc1);
        oscillator_update(&osc2);
        oscillator_update(&osc3);
    }
}


int
main() {
    set_sys_clock_khz(133000, true);

    stdio_init_all();
    tusb_init();

    i2c_init(i2c0, 200000);
    gpio_set_function(8, GPIO_FUNC_I2C);
    gpio_set_function(9, GPIO_FUNC_I2C);
    gpio_pull_up(8);
    gpio_pull_up(9);

    mcp4822_init(&ddac1);
    oscillator_init(&osc1);

    mcp4822_init(&ddac2);
    oscillator_init(&osc2);

    mcp4822_init(&ddac3);
    oscillator_init(&osc3);

    ssd1306_init(&display);

    controller_init(&display, &osc1, &osc2, &osc3);

    multicore_launch_core1(main_core1);

    while (1) {
        tud_task();
        controller_midi_task();
    }
}
