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
    hard_assert(tui->eeprom.base_address);

    tui->eeprom._backlog_idx = 0;

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
ps_tui_eeprom_read(ps_tui_t *tui, void *data, size_t data_len)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);
    hard_assert(tui->eeprom.base_address);

    size_t addr = data - tui->eeprom.base_address;
    hard_assert(addr < tui->eeprom.max_offset);

    uint8_t a[] = {
        addr >> 8,
        addr,
    };
    int rv = i2c_write_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, a, sizeof(a), false);
    if (rv < 0)
        return rv;

    if (data_len == i2c_read_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, data, data_len, false))
        return PICO_OK;

    return PICO_ERROR_GENERIC;
}


static inline int
ack_polling(ps_tui_t *tui)
{
    // we need to send at least a byte, to make the i2c driver happy
    uint8_t tmp = 0;

    int rv = 0;
    while (0 > (rv = i2c_write_blocking(tui->i2c.i2c, tui->eeprom.i2c_address, &tmp, 1, true))) {
        if (rv != PICO_ERROR_GENERIC)
            return rv;
    }
    return rv;
}


static int
eeprom_write(ps_tui_t *tui, size_t addr, void *data, size_t data_len)
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

        rv = ack_polling(tui);
        if (rv < 0)
            return rv;
    }

    return PICO_OK;
}


int
ps_tui_eeprom_write(ps_tui_t *tui, void *data, size_t data_len)
{
    hard_assert(tui);
    hard_assert(tui->eeprom.base_address);

    size_t addr = data - tui->eeprom.base_address;
    hard_assert(addr < tui->eeprom.max_offset);

    return eeprom_write(tui, addr, data, data_len);
}


void
ps_tui_eeprom_write_lazy(ps_tui_t *tui, void *data, size_t data_len)
{
    hard_assert(tui);
    hard_assert(tui->eeprom.base_address);

    size_t addr = data - tui->eeprom.base_address;
    hard_assert(addr < tui->eeprom.max_offset);

    if (tui->eeprom._backlog_idx >= PS_TUI_EEPROM_BACKLOG_SIZE)
        return;

    // avoid duplicates
    for (size_t i = 0; i < tui->eeprom._backlog_idx; i++)
        if (tui->eeprom._backlog[i].addr == addr && tui->eeprom._backlog[i].len == data_len)
            return;

    tui->eeprom._backlog[tui->eeprom._backlog_idx].addr = addr;
    tui->eeprom._backlog[tui->eeprom._backlog_idx++].len = data_len;
}


int
ps_tui_eeprom_sync(ps_tui_t *tui)
{
    hard_assert(tui);
    hard_assert(tui->eeprom.base_address);

    // if backlog overflowed, just push everything
    if (tui->eeprom._backlog_idx > PS_TUI_EEPROM_BACKLOG_SIZE)
        return eeprom_write(tui, 0, tui->eeprom.base_address, tui->eeprom.max_offset);

    int rv = PICO_OK;
    for (size_t i = 0; i < tui->eeprom._backlog_idx; i++) {
        void *data = tui->eeprom.base_address + tui->eeprom._backlog[i].addr;
        rv = eeprom_write(tui, tui->eeprom._backlog[i].addr, data, tui->eeprom._backlog[i].len);
        if (rv != PICO_OK)
            return rv;
    }
    tui->eeprom._backlog_idx = 0;
    return rv;
}


int
ps_tui_eeprom_erase(ps_tui_t *tui)
{
    hard_assert(tui);

    uint8_t buf[tui->eeprom._page_size];
    for (uint8_t i = 0; i < sizeof(buf); i++)
        buf[i] = 0xff;

    // FIXME: ensure that every eeprom model comes with 512 pages.
    for (uint16_t i = 0; i < 512; i++) {
        int rv = eeprom_write(tui, i * tui->eeprom._page_size, buf, sizeof(buf));
        if (rv < 0)
            return rv;
    }

    return PICO_OK;
}
