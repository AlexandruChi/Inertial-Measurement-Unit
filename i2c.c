#include "i2c.h"
#include <hardware/gpio.h>

uint i2c_set_up(i2c_inst_t *i2c, uint scl, uint sda, uint baudrate) {
    uint realBaudrate = i2c_init(i2c, baudrate);

    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_set_function(sda, GPIO_FUNC_I2C);

    gpio_pull_up(scl);
    gpio_pull_up(sda);

    return realBaudrate;
}

void i2c_set_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t val) {
    uint8_t buffer[2] = {reg, val};
    i2c_write_blocking(i2c, addr, buffer, 2, false);
}

void i2c_read_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t *val) {
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    i2c_read_blocking(i2c, addr, val, 1, false);
}