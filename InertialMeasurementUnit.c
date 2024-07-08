#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "MPU9250/MPU9250.h"
#include "i2c.h"
#include "units.h"

#define loop while(1)

void mpu9250SetUp(i2c_inst_t *i2c, uint8_t addr);
void getMpu9250Data(i2c_inst_t *i2c, uint8_t addr, void *data);

int main() {
    if (stdio_init_all()) {
        printf("stdio init failed");
        return -1;
    }

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    i2c_set_up(PICO_DEFAULT_I2C_INSTANCE, PICO_DEFAULT_I2C_SCL_PIN, PICO_DEFAULT_I2C_SDA_PIN, kHz(400));

    mpu9250SetUp(PICO_DEFAULT_I2C_INSTANCE, MPU9250_ADDRESS);

    int16_t data[7];

    double acceleration[3];
    double temperature;
    double rotation[3];

    loop {
        sleep_ms(500);

        getMpu9250Data(PICO_DEFAULT_I2C_INSTANCE, MPU9250_ADDRESS, data);

        for (uint8_t i = 0; i < 3; i++) {
            acceleration[i] = scale_2g_lsb * data[i];
        }

        temperature = TEMP_degC(data[3]);

        for (uint8_t i = 0; i < 3; i++) {
            rotation[i] = scale_250dps_lsb * data[i + 4];
        }

        if (!printf("acc => x: %10.3lf y: %10.3lf z: %10.3lf\nrot => x: %10.3lf y: %10.3lf z: %10.3lf\ntem => t: %10.3lf\n\n", acceleration[0], acceleration[1], acceleration[2], rotation[0], rotation[1], rotation[2], temperature)) {
            break;
        }
    }

    return 0;
}

void mpu9250SetUp(i2c_inst_t *i2c, uint8_t addr) {
    /* Reset device
     * DEVICE_RESET = 1 */
    i2c_set_register(i2c, addr, PWR_MGMT_1, 1 << H_RESET);
    sleep_ms(100);

    /* Wake up device
     * SLEEP = 0 */
    i2c_set_register(i2c, addr, PWR_MGMT_1, 0);
    sleep_ms(100);
}

void getMpu9250Data(i2c_inst_t *i2c, uint8_t addr, void *data) {
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    i2c_read_blocking(i2c, addr, data, 14, false);
}
