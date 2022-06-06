/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico-synth/tui.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include "driver-ec11.h"
#include "driver-eeprom.h"
#include "driver-oled.h"


int
ps_tui_init(ps_tui_t *tui)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);

    i2c_init(tui->i2c.i2c, 400000);
    gpio_set_function(tui->i2c.basepin, GPIO_FUNC_I2C);
    gpio_set_function(tui->i2c.basepin + 1, GPIO_FUNC_I2C);
    gpio_pull_up(tui->i2c.basepin);
    gpio_pull_up(tui->i2c.basepin + 1);

    ec11_init(tui);

    int rv = eeprom_init(tui);
    if (rv != PICO_OK)
        return rv;

    return oled_init(tui);
}


int
ps_tui_task(ps_tui_t *tui)
{
    hard_assert(tui);

    ec11_task(tui);

    return PICO_OK;
}
