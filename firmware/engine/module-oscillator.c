#include <pico-synth/engine/module-oscillator.h>
#include "module-oscillator-data.h"


void
ps_engine_module_oscillator_set_waveform(ps_engine_module_oscillator_ctx_t *ctx,
                                         ps_engine_module_oscillator_waveform_t wf)
{
    if (ctx == NULL || wf >= PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM__LAST)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_wf = wf;
    ctx->_sync = true;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_oscillator_set_note(ps_engine_module_oscillator_ctx_t *ctx, uint8_t note)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_note = ps_engine_get_note(note);
    ctx->_sync = ctx->_note != NULL;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_oscillator_sync(ps_engine_module_oscillator_ctx_t *ctx)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_sync = true;
    mutex_exit(&ctx->_mtx);
}


static void
init(ps_engine_module_oscillator_ctx_t *ctx)
{
    hard_assert(ctx);
    mutex_init(&ctx->_mtx);
}


static int16_t
__not_in_flash_func(sample)(ps_engine_phase_t *p, ps_engine_module_oscillator_ctx_t *ctx)
{
    if (p == NULL || ctx == NULL || ctx->_note == NULL)
        return 0;

    mutex_enter_blocking(&ctx->_mtx);

    if (ctx->_sync) {
        p->pint = 0;
        p->pfrac = 0;
        ctx->_sync = false;
    }
    else {
        uint16_t pfrac = p->pfrac;
        p->pint += ctx->_note->step.pint;
        p->pfrac += ctx->_note->step.pfrac;
        if (p->pfrac < pfrac)
            p->pint++;
        if (p->pint >= waveform_samples_per_cycle)
            p->pint -= waveform_samples_per_cycle;
    }

    int16_t rv = 0;

    switch (ctx->_wf) {
    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE:
        rv = square_table[p->pint];
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE:
        rv = sine_table[p->pint];
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_TRIANGLE:
        rv = triangle_table[p->pint];
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_RIGHT_SAW:
        rv = sawtooth_table[p->pint];
        break;

    case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_LEFT_SAW:
        rv = -sawtooth_table[p->pint];
        break;

    default:
        break;
    }

    mutex_exit(&ctx->_mtx);

    return rv;
}


const ps_engine_module_source_t ps_engine_module_oscillator = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_source_sample_func_t) sample,
};
