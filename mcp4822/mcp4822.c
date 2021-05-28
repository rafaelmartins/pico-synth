#include <pico/assert.h>
#include <pico/stdlib.h>
#include <mcp4822.h>
#include <mcp4822.pio.h>


void
mcp4822_init(mcp4822_t *m)
{
    hard_assert(m);
    hard_assert(m->pio);

    uint offs = pio_add_program(m->pio, &mcp4822_program);
    mcp4822_program_init(m->pio, m->sm, offs, m->basepin);

    while (!mcp4822_unset(m));
}


static inline bool
__not_in_flash_func(_set)(mcp4822_t *m, uint16_t data_a, uint16_t data_b)
{
    hard_assert(m);
    hard_assert(m->pio);

    if (pio_sm_is_tx_fifo_full(m->pio, m->sm))
        return false;

    // as the pico operates on 3v3, we can't enable 2x gain.
    // we have an offset for every output, then we can't use the dac shutdown.
    uint32_t cmd =
        // !A/B   | !GA       | !SHDN     | D11:D0
        (0 << 31) | (1 << 29) | (1 << 28) | ((data_a & 0xfff) << 16) |
        (1 << 15) | (1 << 13) | (1 << 12) |  (data_b & 0xfff);

    m->pio->txf[m->sm] = cmd;
    return true;
}


bool
mcp4822_set(mcp4822_t *m, uint16_t data_a, uint16_t data_b)
{
    return _set(m, data_a, data_b);
}


bool
mcp4822_unset(mcp4822_t *m)
{
    return _set(m, 2047, 2047);
}


void
mcp4822_set_clkdiv(mcp4822_t *m, uint16_t div_int, uint8_t div_frac)
{
    pio_sm_set_clkdiv_int_frac(m->pio, m->sm, div_int, div_frac);
}
