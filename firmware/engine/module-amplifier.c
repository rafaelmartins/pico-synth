#include <pico-synth/engine/module-amplifier.h>


void
ps_engine_module_amplifier_set_gate(ps_engine_module_amplifier_ctx_t *ctx, uint8_t gain)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_gain = gain;
    ctx->_gate = true;
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_amplifier_unset_gate(ps_engine_module_amplifier_ctx_t *ctx)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    ctx->_gate = false;
    mutex_exit(&ctx->_mtx);
}


static void
init(ps_engine_module_amplifier_ctx_t *ctx)
{
    hard_assert(ctx);
    mutex_init(&ctx->_mtx);
}


static int16_t
__not_in_flash_func(sample)(int16_t in, const ps_engine_phase_t *p, ps_engine_module_amplifier_ctx_t *ctx)
{
    if (p == NULL || ctx == NULL || !ctx->_gate)
        return 0;

    return in * ctx->_gain / 0x7f;
}


const ps_engine_module_filter_t ps_engine_module_amplifier = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_filter_sample_func_t) sample,
};
