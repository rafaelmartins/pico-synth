/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hardware/gpio.h>
#include <hardware/timer.h>
#include "driver-ec11.h"

typedef enum {
    EC11_STATE_INITIAL = 1,
    EC11_STATE_FIRST_CW,
    EC11_STATE_FIRST_CCW,
    EC11_STATE_SECOND_CW,
    EC11_STATE_SECOND_CCW,
    EC11_STATE_CW,
    EC11_STATE_CCW,
} ec11_state_t;


void
ec11_init(ps_tui_t *tui)
{
    hard_assert(tui);

    gpio_set_dir(tui->encoder.button, false);
    gpio_set_dir(tui->encoder.a, false);
    gpio_set_dir(tui->encoder.b, false);
    gpio_pull_up(tui->encoder.button);
    gpio_pull_up(tui->encoder.a);
    gpio_pull_up(tui->encoder.b);

    tui->encoder._button_hit = 0;
    tui->encoder._rotate_hit = 0;
    tui->encoder._rotate_state = EC11_STATE_INITIAL;
    tui->encoder._a_value = true;
    tui->encoder._b_value = true;
}


static inline void
button_task(ps_tui_t *tui)
{
    bool button = gpio_get(tui->encoder.button);
    if (button)
        return;

    uint64_t t = time_us_64();
    if (t - tui->encoder._button_hit <= tui->encoder.button_debounce_us)
        return;
    tui->encoder._button_hit = t;

    ec11_callback(tui, PS_TUI_ENCODER_ACTION_BUTTON);
}


static inline void
rotate_task(ps_tui_t *tui)
{
    bool a = gpio_get(tui->encoder.a);
    bool b = gpio_get(tui->encoder.b);
    if (a == tui->encoder._a_value && b == tui->encoder._b_value)
        return;

    uint64_t t = time_us_64();
    if (t - tui->encoder._rotate_hit <= tui->encoder.rotate_debounce_us)
        return;
    tui->encoder._rotate_hit = t;

    tui->encoder._a_value = a;
    tui->encoder._b_value = b;

    switch ((ec11_state_t) tui->encoder._rotate_state) {
    case EC11_STATE_INITIAL:
        if (!a)
            tui->encoder._rotate_state = EC11_STATE_FIRST_CW;
        else if (!b)
            tui->encoder._rotate_state = EC11_STATE_FIRST_CCW;
        break;

    case EC11_STATE_FIRST_CW:
        if (a && b)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (!(a || b))
            tui->encoder._rotate_state = EC11_STATE_SECOND_CW;
        break;

    case EC11_STATE_FIRST_CCW:
        if (a && b)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (!(a || b))
            tui->encoder._rotate_state = EC11_STATE_SECOND_CCW;
        break;

    case EC11_STATE_SECOND_CW:
        if (a && b)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (!b)
            tui->encoder._rotate_state = EC11_STATE_CW;
        break;

    case EC11_STATE_SECOND_CCW:
        if (a && b)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (!a)
            tui->encoder._rotate_state = EC11_STATE_CCW;
        break;

    case EC11_STATE_CW:
        if (!b)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (a && b) {
            ec11_callback(tui, PS_TUI_ENCODER_ACTION_ROTATE_CW);
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        }
        break;

    case EC11_STATE_CCW:
        if (!a)
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        else if (a && b) {
            ec11_callback(tui, PS_TUI_ENCODER_ACTION_ROTATE_CCW);
            tui->encoder._rotate_state = EC11_STATE_INITIAL;
        }
        break;
    }
}


void
ec11_task(ps_tui_t *tui)
{
    hard_assert(tui);

    button_task(tui);
    rotate_task(tui);
}
