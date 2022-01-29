#pragma once

#include <hardware/i2c.h>

typedef struct {
    i2c_inst_t *i2c;
    uint8_t i2c_address;
    enum {
        EEPROM_I2C_24LC256 = 1,
        EEPROM_I2C_24LC512,
    } ic;

    // private
    size_t page_size;
} eeprom_i2c_t;

int eeprom_i2c_init(eeprom_i2c_t *m);
int eeprom_i2c_read(eeprom_i2c_t *m, uint16_t addr, uint8_t *data, size_t data_len);
int eeprom_i2c_write(eeprom_i2c_t *m, uint16_t addr, uint8_t *data, size_t data_len);
