#ifndef I2C_H
#define I2C_H

#include "hardware/i2c.h"

uint i2c_set_up(i2c_inst_t *i2c, uint scl, uint sda, uint baudrate);
void i2c_set_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t val);
void i2c_read_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t *val);

#endif /* I2C_H */
