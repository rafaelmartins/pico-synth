#include <string.h>
#include <hardware/i2c.h>
#include <eeprom-i2c.h>


int
eeprom_i2c_init(eeprom_i2c_t *m)
{
    hard_assert(m);
    hard_assert(m->i2c);

    // broadcasts not supported, we assume that address 0 means "use default"
    if (m->i2c_address == 0)
        m->i2c_address = 0x50;

    m->page_size = 0;

    switch (m->ic) {
        case EEPROM_I2C_24LC256:
            m->page_size = 64;
            break;

        case EEPROM_I2C_24LC512:
            m->page_size = 128;
            break;
    }

    return m->page_size == 0 ? PICO_ERROR_GENERIC : PICO_OK;  // FIXME
}


int
eeprom_i2c_read(eeprom_i2c_t *m, uint16_t addr, uint8_t *data, size_t data_len)
{
    hard_assert(m);
    hard_assert(m->i2c);

    uint8_t a[] = {
        addr >> 8,
        addr,
    };
    int rv = i2c_write_blocking(m->i2c, m->i2c_address, a, sizeof(a), false);
    if (rv < 0)
        return rv;

    return i2c_read_blocking(m->i2c, m->i2c_address, data, data_len, false);
}


int
eeprom_i2c_write(eeprom_i2c_t *m, uint16_t addr, uint8_t *data, size_t data_len)
{
    hard_assert(m);
    hard_assert(m->i2c);
    hard_assert(m->page_size);

    uint8_t a[m->page_size + 2];  // FIXME for >=1Mb devices
    size_t idx = 0;

    // we need to align with the end of the eeprom page containing our start address
    size_t to_send = m->page_size - (addr % m->page_size);
    if (to_send > data_len)
        to_send = data_len;

    while (data_len > 0) {
        a[0] = addr >> 8;
        a[1] = addr;
        memcpy(a + 2, data + idx, to_send);

        addr += to_send;
        idx += to_send;
        data_len -= to_send;

        int rv = i2c_write_blocking(m->i2c, m->i2c_address, a, to_send + 2, false);
        if (rv < 0)
            return rv;

        to_send = data_len > m->page_size ? m->page_size : data_len;
        sleep_ms(5);
    }

    return PICO_OK;
}
