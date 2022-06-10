/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico-synth/engine/module-adsr.h>
#include "module-adsr-data.h"


const char*
ps_engine_module_adsr_get_time_description(uint8_t time)
{
    return times[time].description;
}


static inline void
set_state(ps_engine_module_adsr_ctx_t *ctx, ps_engine_module_adsr_state_t state)
{
    ctx->_state = state;
    ctx->_range_start = ctx->_level;

    switch (ctx->_state) {
    case PS_ENGINE_MODULE_ADSR_STATE_ATTACK:
        ctx->_range_end = adsr_amplitude;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_DECAY:
        ctx->_range_end = ctx->_sustain;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_SUSTAIN:
        ctx->_range_end = ctx->_level;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_RELEASE:
        ctx->_range_end = 0;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_OFF:
        break;
    }

    if (ctx->_range_start > adsr_amplitude)
        ctx->_range_start = adsr_amplitude;

    if (ctx->_range_end > adsr_amplitude)
        ctx->_range_end = adsr_amplitude;

    ps_engine_phase_reset(&ctx->_phase);
}


void
ps_engine_module_adsr_set_attack(ps_engine_module_adsr_ctx_t *ctx, uint8_t attack)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_attack = attack;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_adsr_set_decay(ps_engine_module_adsr_ctx_t *ctx, uint8_t decay)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_decay = decay;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_adsr_set_sustain(ps_engine_module_adsr_ctx_t *ctx, uint8_t sustain)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_sustain = sustain;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_adsr_set_release(ps_engine_module_adsr_ctx_t *ctx, uint8_t release)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_release = release;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_adsr_set_gate(ps_engine_module_adsr_ctx_t *ctx)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    set_state(ctx, PS_ENGINE_MODULE_ADSR_STATE_ATTACK);
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_adsr_unset_gate(ps_engine_module_adsr_ctx_t *ctx)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    set_state(ctx, PS_ENGINE_MODULE_ADSR_STATE_RELEASE);
    mutex_exit(&ctx->_mtx);
}


static void
init(ps_engine_module_adsr_ctx_t *ctx)
{
    hard_assert(ctx);
    mutex_init(&ctx->_mtx);

    ps_engine_module_adsr_set_sustain(ctx, 0xff);
}


static int16_t
__not_in_flash_func(sample)(int16_t in, const ps_engine_phase_t *p, ps_engine_module_adsr_ctx_t *ctx)
{
    if (p == NULL || ctx == NULL)
        return 0;

    mutex_enter_blocking(&ctx->_mtx);

    const uint16_t *table = NULL;

    switch (ctx->_state) {
    case PS_ENGINE_MODULE_ADSR_STATE_ATTACK:
        if (ps_engine_phase_step(&ctx->_phase, &times[ctx->_attack].step, adsr_samples_per_cycle))
            set_state(ctx, PS_ENGINE_MODULE_ADSR_STATE_DECAY);
        table = attack_curve;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_DECAY:
        if (ps_engine_phase_step(&ctx->_phase, &times[ctx->_decay].step, adsr_samples_per_cycle))
            set_state(ctx, PS_ENGINE_MODULE_ADSR_STATE_SUSTAIN);
        else
            table = decay_release_curve;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_RELEASE:
        if (ps_engine_phase_step(&ctx->_phase, &times[ctx->_release].step, adsr_samples_per_cycle)) {
            set_state(ctx, PS_ENGINE_MODULE_ADSR_STATE_OFF);
            mutex_exit(&ctx->_mtx);
            return 0;
        }
        table = decay_release_curve;
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_SUSTAIN:
        break;

    case PS_ENGINE_MODULE_ADSR_STATE_OFF:
        mutex_exit(&ctx->_mtx);
        return 0;
    }

    uint8_t balance = table != NULL ? table[ctx->_phase.pint] : adsr_amplitude;
    ctx->_level = ((ctx->_range_start * (adsr_amplitude - balance)) +
                   (ctx->_range_end * balance)) / adsr_amplitude;
    int32_t sample = in * ctx->_level;

    mutex_exit(&ctx->_mtx);

    return sample >> 8;
}


const ps_engine_module_filter_t ps_engine_module_adsr = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_filter_sample_func_t) sample,
};
