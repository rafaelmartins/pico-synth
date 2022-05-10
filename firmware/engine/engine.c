#include <pico-synth/engine.h>
#include "driver-mcp4822.h"
#include "engine-data.h"


static void
init_channel(ps_engine_channel_t *chan)
{
    if (chan == NULL || chan->voices == NULL)
        return;

    chan->_num_voices = 0;
    for (ps_engine_voice_t *v = chan->voices; v != NULL; v = v->next) {
        if (v->source->mod->init_func != NULL)
            v->source->mod->init_func(v->source->data);

        for (ps_engine_module_filter_ctx_t *c = v->filters; c != NULL; c = c->next)
            if (c->mod != NULL && c->mod->init_func != NULL)
                c->mod->init_func(c->data);

        chan->_num_voices++;

        // if there's no sink, we can't process more than one voice per channel
        if (chan->sink == NULL || chan->sink->mod == NULL)
            break;
    }

    if (chan->sink != NULL && chan->sink->mod != NULL && chan->sink->mod->init_func != NULL)
        chan->sink->mod->init_func(chan->sink->data);
}


int
ps_engine_init(ps_engine_t *e)
{
    hard_assert(e);

    init_channel(&e->channels[0]);
    init_channel(&e->channels[1]);

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
    if (chan == NULL || chan->voices == NULL)
        return waveform_amplitude;

    int16_t in[chan->_num_voices];
    int8_t i = 0;

    for (ps_engine_voice_t *v = chan->voices; v != NULL; v = v->next) {
        if (v->source->mod == NULL || v->source->mod->sample_func == NULL || i >= chan->_num_voices)
            continue;

        int16_t sample = v->source->mod->sample_func(&v->phase, v->source->data);

        for (ps_engine_module_filter_ctx_t *c = v->filters; c != NULL; c = c->next) {
            if (c->mod == NULL || c->mod->sample_func == NULL)
                continue;

            sample = c->mod->sample_func(sample, &v->phase, c->data);
        }

        in[i++] = sample;

        // if there's no sink, we can't process more than one voice per channel
        if (chan->sink == NULL || chan->sink->mod == NULL)
            break;
    }

    int16_t rv = 0;
    if (chan->sink != NULL && chan->sink->mod != NULL && chan->sink->mod->sample_func != NULL)
        rv = chan->sink->mod->sample_func(in, i, chan->sink->data);
    else if (i > 0)
        rv = in[0];

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
    return (eval_channel(&e->channels[0]) << 16) | eval_channel(&e->channels[1]);
}
