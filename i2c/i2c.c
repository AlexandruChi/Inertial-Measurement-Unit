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

bool i2c_set_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t val) {
    uint8_t buffer[2] = {reg, val};
    if (i2c_write_blocking(i2c, addr, buffer, 2, false) == PICO_ERROR_GENERIC) {
        return false;
    }
    return true;
}

bool i2c_read_register(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t *val) {
    if (
            i2c_write_blocking(i2c, addr, &reg, 1, true) == PICO_ERROR_GENERIC ||
            i2c_read_blocking(i2c, addr, val, 1, false) == PICO_ERROR_GENERIC
    ) {
        return false;
    }
    return true;
}

bool i2c_rw_read_register(i2c_inst_t *i2c, uint8_t addr_r, uint8_t addr_w, uint8_t reg, uint8_t *val) {
    if (
            i2c_write_blocking(i2c, addr_w, &reg, 1, true) == PICO_ERROR_GENERIC ||
            i2c_read_blocking(i2c, addr_r, val, 1, false) == PICO_ERROR_GENERIC
            ) {
        return false;
    }
    return true;
}

uint i2c_scan(i2c_inst_t *i2c, uint8_t *addr) {
    uint8_t i = 0, val;

    for (uint8_t x = 0; x < 1 << 7; x++) {
        if (i2c_read_blocking(i2c, x, &val, 1, false) != PICO_ERROR_GENERIC) {
            addr[i++] = x;
        }
    }

    return i;
}