#include <pico-synth/engine/module-mixer.h>


void
ps_engine_module_mixer_set_master_gain(ps_engine_module_mixer_ctx_t *ctx, uint8_t gain)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_master_gain = gain;
    mutex_exit(&ctx->_mtx);
}


static void
init(ps_engine_module_mixer_ctx_t *ctx)
{
    hard_assert(ctx);
    mutex_init(&ctx->_mtx);
}


static int16_t
__not_in_flash_func(sample)(const int16_t *in, uint8_t in_len, ps_engine_module_mixer_ctx_t *ctx)
{
    if (in == NULL || in_len == 0 || ctx == NULL)
        return 0;

    int16_t rv = 0;

    mutex_enter_blocking(&ctx->_mtx);
    for (uint8_t i = 0; i < in_len; i++)
        rv += in[i] * ctx->_master_gain / 0x7f;
    mutex_exit(&ctx->_mtx);

    return rv;
}


const ps_engine_module_sink_t ps_engine_module_mixer = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_sink_sample_func_t) sample,
};
