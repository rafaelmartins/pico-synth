#include <pico-synth/engine/module-oscillator.h>
#include "module-oscillator-data.h"


void
ps_engine_module_oscillator_set_waveform(ps_engine_module_oscillator_ctx_t *ctx,
                                         ps_engine_module_oscillator_waveform_t wf)
{
    if (ctx == NULL || wf >= PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM__LAST)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_wf_next = wf;
    ctx->_wf_set = true;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_oscillator_set_note(ps_engine_module_oscillator_ctx_t *ctx, uint8_t note)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_note_next = ps_engine_get_note(note);
    mutex_exit(&ctx->_mtx);
}


static void
init(ps_engine_module_oscillator_ctx_t *ctx)
{
    hard_assert(ctx);
    mutex_init(&ctx->_mtx);
}


static inline uint16_t
__not_in_flash_func(next_sample_id)(const ps_engine_phase_t *cur)
{
    if (cur->pint + 1 == waveform_samples_per_cycle)
        return cur->pint - waveform_samples_per_cycle + 1;
    return cur->pint + 1;
}


static inline int16_t
__not_in_flash_func(interpolate_sample)(const int16_t *table, const ps_engine_phase_t *cur)
{
    return (table[cur->pint] * (0xffff - cur->pfrac)) / 0xffff +
           (table[next_sample_id(cur)] * cur->pfrac) / 0xffff;
}


static int16_t
__not_in_flash_func(sample)(ps_engine_phase_t *p, ps_engine_module_oscillator_ctx_t *ctx)
{
    if (p == NULL || ctx == NULL)
        return 0;

    mutex_enter_blocking(&ctx->_mtx);

    if (ctx->_note == NULL) {  // not running
        if (ctx->_note_next == NULL || !ctx->_wf_set) {  // no note to play yet
            mutex_exit(&ctx->_mtx);
            return 0;
        }
        ctx->_note = ctx->_note_next;
        ctx->_note_next = NULL;
        ctx->_wf = ctx->_wf_next;
        ctx->_wf_set = false;
        p->pint = 0;
        p->pfrac = 0;
    }
    else {
        uint16_t pfrac = p->pfrac;
        p->pint += ctx->_note->step.pint;
        p->pfrac += ctx->_note->step.pfrac;
        if (p->pfrac < pfrac)
            p->pint++;
        if (p->pint >= waveform_samples_per_cycle) {
            p->pint -= waveform_samples_per_cycle;
            if (ctx->_note_next != NULL) {  // new note to play
                ctx->_note = ctx->_note_next;
                ctx->_note_next = NULL;
            }
            if (ctx->_wf_set) {  // new waveform to set
                ctx->_wf = ctx->_wf_next;
                ctx->_wf_set = false;
            }
        }
    }

    int16_t rv = 0;
    uint8_t octave = ctx->_note->id / 12;

    // we return a sine (that is already bandlimited by definition) for any high
    // octave without an explicit wavetable.
    if (octave >= wavetable_octaves) {
        rv = interpolate_sample(sine_wavetable, p);
    }
    else {
        switch (ctx->_wf) {
        case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SQUARE:
            rv = interpolate_sample(square_wavetables[octave], p);
            break;

        case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_SINE:
            rv = interpolate_sample(sine_wavetable, p);
            break;

        case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_TRIANGLE:
            rv = interpolate_sample(triangle_wavetables[octave], p);
            break;

        case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_RIGHT_SAW:
            rv = -interpolate_sample(sawtooth_wavetables[octave], p);
            break;

        case PS_ENGINE_MODULE_OSCILLATOR_WAVEFORM_LEFT_SAW:
            rv = interpolate_sample(sawtooth_wavetables[octave], p);
            break;

        default:
            break;
        }
    }

    mutex_exit(&ctx->_mtx);

    return rv;
}


const ps_engine_module_source_t ps_engine_module_oscillator = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_source_sample_func_t) sample,
};
