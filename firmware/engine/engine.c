#include <pico-synth/engine.h>
#include "driver-mcp4822.h"
#include "engine-data.h"


static void
init_channel(ps_engine_channel_t *chan)
{
    if (chan == NULL || chan->source->mod == NULL)
        return;

    if (chan->source->mod->init_func != NULL)
        chan->source->mod->init_func(chan->source->data);

    for (ps_engine_module_filter_ctx_t *c = chan->filters; c != NULL; c = c->next) {
        if (c->mod == NULL || c->mod->init_func == NULL)
            continue;

        c->mod->init_func(c->data);
    }
}


int
ps_engine_init(ps_engine_t *e)
{
    hard_assert(e);

    init_channel(&e->channel[0]);
    init_channel(&e->channel[1]);

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
__not_in_flash_func(eval_channel)(ps_engine_channel_t *chan)
{
    if (chan == NULL || chan->source->mod == NULL || chan->source->mod->sample_func == NULL)
        return waveform_amplitude;

    int16_t rv = chan->source->mod->sample_func(&chan->phase, chan->source->data);

    for (ps_engine_module_filter_ctx_t *c = chan->filters; c != NULL; c = c->next) {
        if (c->mod == NULL || c->mod->sample_func == NULL)
            continue;

        rv = c->mod->sample_func(rv, &chan->phase, c->data);
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
    return (eval_channel(&e->channel[0]) << 16) | eval_channel(&e->channel[1]);
}
