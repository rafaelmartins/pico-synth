#include <pico/stdlib.h>
#include <driver-mcp4822.pio.h>
#include "driver-mcp4822-data.h"
#include "driver-mcp4822.h"


void
mcp4822_init(ps_engine_t *p)
{
    hard_assert(p);
    hard_assert(p->dac.pio);

    const pio_program_t *prog;
    pio_sm_config (*prog_get_default_config)(uint offset);
    uint offset_ep;
    uint pin_count;

    if (p->dac.with_ldac) {
        prog = &mcp4822_ldac_program;
        prog_get_default_config = mcp4822_ldac_program_get_default_config;
        offset_ep = mcp4822_ldac_offset_ep;
        pin_count = 4;
    }
    else {
        prog = &mcp4822_program;
        prog_get_default_config = mcp4822_program_get_default_config;
        offset_ep = mcp4822_offset_ep;
        pin_count = 3;
    }

    uint offset = pio_add_program(p->dac.pio, prog);
    for (uint i = 0; i < pin_count; i++)
        pio_gpio_init(p->dac.pio, p->dac.basepin + i);
    pio_sm_set_consecutive_pindirs(p->dac.pio, p->dac.state_machine, p->dac.basepin, pin_count, true);
    pio_sm_config c = prog_get_default_config(offset);
    sm_config_set_sideset_pins(&c, p->dac.basepin + 1);
    sm_config_set_out_pins(&c, p->dac.basepin, 1);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_out_shift(&c, false, true, 32);
    pio_sm_init(p->dac.pio, p->dac.state_machine, offset + offset_ep, &c);
    pio_sm_exec(p->dac.pio, p->dac.state_machine, pio_encode_set(pio_y, 15));
    pio_sm_set_clkdiv(p->dac.pio, p->dac.state_machine, mcp4822_clkdiv / 2);
    pio_sm_set_enabled(p->dac.pio, p->dac.state_machine, true);
}


void
mcp4822_task(ps_engine_t *p)
{
    hard_assert(p);
    hard_assert(p->dac.pio);

    if (mcp4822_callback == NULL || pio_sm_is_tx_fifo_full(p->dac.pio, p->dac.state_machine))
        return;

    pio_sm_put(p->dac.pio, p->dac.state_machine, (mcp4822_callback(p) & ((0xfff << 16) | 0xfff)) |
        // !A/B   | !GA       | !SHDN     | !A/B      | !GA       | !SHDN
        (0 << 31) | (1 << 29) | (1 << 28) | (1 << 15) | (1 << 13) | (1 << 12));
}
