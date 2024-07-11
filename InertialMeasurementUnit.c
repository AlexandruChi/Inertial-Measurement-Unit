#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "MPU/MPU6500.h"
#include "AK8963/AK8963.h"

#include "i2c.h"
#include "units.h"

#define loop while(1)

#define BAUDRATE kHz(400)

void mpu6500Start(i2c_dev_t device);
void mpu6500SetScale(i2c_dev_t device);
void mpu6500GetCurrentData(i2c_dev_t device, void *data);

void ak8963Start(i2c_dev_t device);

int main() {
    if (!stdio_init_all()) {
        printf("stdio init failed");
        return -1;
    }

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    uint baudrate = i2c_set_up(PICO_DEFAULT_I2C_INSTANCE, PICO_DEFAULT_I2C_SCL_PIN, PICO_DEFAULT_I2C_SDA_PIN, BAUDRATE);
    if (baudrate != BAUDRATE) {
        printf("baudrate set to %u instead of %u", baudrate, BAUDRATE);
    }

    i2c_dev_t mpu9250 = {PICO_DEFAULT_I2C_INSTANCE, MPU_ADDRESS};

    mpu6500Start(mpu9250);

    int8_t data[14];

    double acceleration[3];
    double rotation[3];

    loop {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

        mpu6500GetCurrentData(mpu9250, data);

        for (uint8_t i = 0; i < 3; i++) {
            acceleration[i] = scale_lsb_2g * ((data[i * 2] << 8) + data[i * 2 + 1]);
        }

        for (uint8_t i = 0; i < 3; i++) {
            rotation[i] = scale_lsb_250dps * ((data[(i + 4) * 2] << 8) + data[(i + 4) * 2 + 1]);
        }

        if (!printf("acc => x: %7.2lf\ty: %7.2lf\tz: %7.2lf\nrot => x: %7.2lf\ty: %7.2lf\tz: %7.2lf\n\n", acceleration[0], acceleration[1], acceleration[2], rotation[0], rotation[1], rotation[2])) {
            break;
        }

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(1000);
    }

    return 0;
}

void mpu6500Start(i2c_dev_t device) {
    /* Reset device
     * DEVICE_RESET = 1 */
    i2c_set_register(device.i2c, device.addr, PWR_MGMT_1, 1 << H_RESET);
    sleep_ms(100);

    /* Wake up device and disable temperature
     * SLEEP = 0
     * PD_PTAT = 1 */
    i2c_set_register(device.i2c, device.addr, PWR_MGMT_1, 1 << PD_PTAT);
    sleep_ms(100);

    /* Enable FIFO
     * GYRO_XOUT = 1
     * GYRO_YOUT = 1
     * GYRO_ZOUT = 1
     * ACCEL = 1 */
    i2c_set_register(device.i2c, device.addr, FIFO_EN, 1 << GYRO_XOUT | 1 << GYRO_YOUT | 1 << GYRO_ZOUT | 1 << ACCEL);
    sleep_ms(100);
}

void mpu6500GetCurrentData(i2c_dev_t device, void *data) {
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(device.i2c, device.addr, &reg, 1, true);
    i2c_read_blocking(device.i2c, device.addr, data, 14, false);
}

void ak8963Start(i2c_dev_t device) {
    i2c_set_register(device.i2c, device.addr, CNTL, 2);
    sleep_ms(100);
}
