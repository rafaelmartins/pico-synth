#include <pico/assert.h>
#include <pico/stdlib.h>
#include <mcp4822.h>
#include <mcp4822.pio.h>


void
mcp4822_init(mcp4822_t *m)
{
    hard_assert(m);
    hard_assert(m->pio);

    mutex_init(&m->mtx);
    m->cb[0] = NULL;
    m->cb[1] = NULL;
    m->cb_data[0] = NULL;
    m->cb_data[1] = NULL;
    m->data[0] = mcp4822_offset;
    m->data[1] = mcp4822_offset;
    m->resend = false;

    if (m->dual) {
        uint offs = pio_add_program(m->pio, &mcp4822_dual_program);
        mcp4822_dual_program_init(m->pio, m->sm, offs, m->basepin, m->clkdiv / 2);
    }
    else {
        uint offs = pio_add_program(m->pio, &mcp4822_single_program);
        mcp4822_single_program_init(m->pio, m->sm, offs, m->basepin, m->clkdiv);
    }
}


static inline bool
__not_in_flash_func(_set)(mcp4822_t *m)
{
    if (pio_sm_is_tx_fifo_full(m->pio, m->sm))
        return false;

    // as the pico operates on 3v3, we can't enable 2x gain.
    // we have an offset for every output, then we can't use the dac shutdown.
    if (m->dual) {
        uint32_t cmd =
            // !A/B   | !GA       | !SHDN     | D11:D0
            (0 << 31) | (1 << 29) | (1 << 28) | (((m->data[0] + mcp4822_offset) & 0xfff) << 16) |
            (1 << 15) | (1 << 13) | (1 << 12) |  ((m->data[1] + mcp4822_offset) & 0xfff);

        *(io_rw_32 *)&m->pio->txf[m->sm] = cmd;
    }
    else {
        uint16_t cmd =
            // !A/B   | !GA       | !SHDN     | D11:D0
            (0 << 15) | (1 << 13) | (1 << 12) | ((m->data[0] + mcp4822_offset) & 0xfff);

        *(io_rw_16 *)&m->pio->txf[m->sm] = cmd;
    }

    return true;
}


bool
mcp4822_update(mcp4822_t *m)
{
    hard_assert(m);
    hard_assert(m->pio);

    mutex_enter_blocking(&m->mtx);

    if (m->cb[0] == NULL && m->cb[1] == NULL) {
        mutex_exit(&m->mtx);
        return false;
    }

    if (!m->resend) {
        if (m->cb[0] != NULL)
            m->data[0] = m->cb[0](m->cb_data[0]);

        if (m->dual && m->cb[1] != NULL)
            m->data[1] = m->cb[1](m->cb_data[1]);
    }

    mutex_exit(&m->mtx);

    m->resend = !_set(m);
    return !m->resend;
}


void
mcp4822_set_cb(mcp4822_t *m, mcp4822_dac_t dac, mcp4822_sample_cb_t cb)
{
    hard_assert(m);

    mutex_enter_blocking(&m->mtx);
    m->cb[dac] = cb;
    mutex_exit(&m->mtx);
}


void
mcp4822_set_cb_data(mcp4822_t *m, mcp4822_dac_t dac, void *cb_data)
{
    hard_assert(m);

    mutex_enter_blocking(&m->mtx);
    m->cb_data[dac] = cb_data;
    mutex_exit(&m->mtx);
}
