/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico-synth/engine.h>


void
__not_in_flash_func(ps_engine_phase_reset)(ps_engine_phase_t *phase)
{
    if (phase == NULL)
        return;

    phase->data = 0;
}


bool
__not_in_flash_func(ps_engine_phase_step)(ps_engine_phase_t *phase, const ps_engine_phase_t *step, uint16_t table_len)
{
    if (phase == NULL || step == NULL)
        return false;

    phase->data += step->data;
    if (phase->pint >= table_len) {
        phase->pint -= table_len;
        return true;
    }
    return false;
}


uint16_t
__not_in_flash_func(ps_engine_phase_next_pint)(const ps_engine_phase_t *phase, uint16_t table_len)
{
    if (phase == NULL)
        return 0;

    return 1 + ((phase->pint + 1 >= table_len) ? phase->pint - table_len : phase->pint);
}
