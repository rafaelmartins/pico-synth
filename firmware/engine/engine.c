#include <pico-synth/engine.h>
#include "driver-mcp4822.h"
#include "engine-data.h"


static void
init_channel(ps_engine_module_ctx_t *root)
{
    for (ps_engine_module_ctx_t *c = root; c != NULL; c = c->next) {
        if (c->mod == NULL || c->mod->init_func == NULL)
            continue;

        c->mod->init_func(c->data);
    }
}


int
ps_engine_init(ps_engine_t *e)
{
    hard_assert(e);

    init_channel(e->channel[0].root);
    init_channel(e->channel[1].root);

    mcp4822_init(e);
    return PICO_OK;
}


int
ps_engine_task(ps_engine_t *e)
{
    hard_assert(e);

    mcp4822_task(e);
    return PICO_OK;
}


static inline uint16_t
__not_in_flash_func(eval_channel)(ps_engine_module_ctx_t *root, ps_engine_phase_t *phase)
{
    int16_t rv = 0;
    for (ps_engine_module_ctx_t *c = root; c != NULL; c = c->next) {
        if (c->mod == NULL || c->mod->sample_func == NULL)
            continue;

        rv = c->mod->sample_func(rv, phase, c->data);
    }

    if (rv < -waveform_amplitude)
        return 0;
    if (rv > waveform_amplitude)
        return 0xfff;
    return rv + waveform_amplitude;
}


uint32_t
__not_in_flash_func(mcp4822_callback)(ps_engine_t *e)
{
    // the caller will ensure that engine is not null
    return (eval_channel(e->channel[0].root, &e->channel[0].phase) << 16) |
            eval_channel(e->channel[1].root, &e->channel[1].phase);
}
