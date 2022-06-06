/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "driver-eeprom.h"


int
eeprom_init(ps_tui_t *tui)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);

    // broadcasts not supported, we assume that address 0 means "use default"
    if (tui->eeprom.i2c_address == 0)
        tui->eeprom.i2c_address = 0x50;

    tui->eeprom._page_size = 0;

    switch (tui->eeprom.ic) {
        case PS_TUI_EEPROM_24LC256:
            tui->eeprom._page_size = 64;
            break;

        case PS_TUI_EEPROM_24LC512:
            tui->eeprom._page_size = 128;
            break;
    }

    return tui->eeprom._page_size == 0 ? PICO_ERROR_GENERIC : PICO_OK;  // FIXME
}


int
ps_tui_eeprom_read(ps_tui_t *tui, uint16_t addr, uint8_t *data, size_t data_len)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);

    uint8_t a[] = {
        addr >> 8,
        addr,
    };
    int rv = i2c_write_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, a, sizeof(a), false);
    if (rv < 0)
        return rv;

    return i2c_read_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, data, data_len, false);
}


int
ps_tui_eeprom_write(ps_tui_t *tui, uint16_t addr, uint8_t *data, size_t data_len)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);
    hard_assert(tui->eeprom._page_size);

    uint8_t a[tui->eeprom._page_size + 2];  // FIXME for >=1Mb devices
    size_t idx = 0;

    // we need to align with the end of the eeprom page containing our start address
    size_t to_send = tui->eeprom._page_size - (addr % tui->eeprom._page_size);
    if (to_send > data_len)
        to_send = data_len;

    while (data_len > 0) {
        a[0] = addr >> 8;
        a[1] = addr;
        memcpy(a + 2, data + idx, to_send);

        addr += to_send;
        idx += to_send;
        data_len -= to_send;

        int rv = i2c_write_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, a, to_send + 2, false);
        if (rv < 0)
            return rv;

        to_send = data_len > tui->eeprom._page_size ? tui->eeprom._page_size : data_len;
        sleep_ms(5);
    }

    return PICO_OK;
}
