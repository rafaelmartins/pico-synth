#include <pico-synth/engine/module-amplifier.h>


void
ps_engine_module_amplifier_set_gate(ps_engine_module_amplifier_ctx_t *ctx, uint8_t gain)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    switch (ctx->_gate) {
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF:
        ctx->_gain_next = gain;
        ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON;
        break;

    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF:
        ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN;
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN:
        ctx->_gain_next = gain;
        break;
    }
    mutex_exit(&ctx->_mtx);
}


void
ps_engine_module_amplifier_unset_gate(ps_engine_module_amplifier_ctx_t *ctx)
{
    if (ctx == NULL)
        return;

    mutex_enter_blocking(&ctx->_mtx);
    switch (ctx->_gate) {
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF:
        break;

    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON:
        ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF;
        break;

    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN:
        ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF;
        break;
    }
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
    if (p == NULL || ctx == NULL)
        return 0;

    mutex_enter_blocking(&ctx->_mtx);

    if ((p->pint == 0 && p->pfrac == 0) || (p->pint < ctx->_pint)) {  // phase ~0
        switch (ctx->_gate) {
        case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON:
        case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN:
            ctx->_gain = ctx->_gain_next;
            ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON;
            break;

        case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF:
            ctx->_gate = PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF;
            break;

        case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON:
        case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF:
            break;
        }
    }

    int16_t rv = 0;

    switch (ctx->_gate) {
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_OFF:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_ON:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_CHANGE_GAIN:
        rv = in * ctx->_gain / 0x7f;
        break;

    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_TO_ON:
    case PS_ENGINE_MODULE_AMPLIFIER_GATE_STATE_OFF:
        break;
    }

    ctx->_pint = p->pint;
    mutex_exit(&ctx->_mtx);

    return rv;
}


const ps_engine_module_filter_t ps_engine_module_amplifier = {
    .init_func = (ps_engine_module_init_func_t) init,
    .sample_func = (ps_engine_module_filter_sample_func_t) sample,
};
