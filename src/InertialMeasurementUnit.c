#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "MPU/MPU6500_Reg.h"
#include "src/BMP180/BMP180_Reg.h"

#include "units.h"
#include "Barometer.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#define loop while(1)

#define BAUDRATE kHz(400)

#define p0 hPa(1013.25)

void mpuStart(i2c_dev_t device);
void mpuCalibrate(i2c_dev_t device);
void mpuCurrentData(i2c_dev_t device, void *data);
void mpuConfigFIFO(i2c_dev_t device, bool gyro, bool accel);
void mpuResetFIFO(i2c_dev_t device);
uint16_t mpuSizeFIFO(i2c_dev_t device);
void mpuReadFIFO(i2c_dev_t device, void *data, uint16_t size);
double getAltitude(double p);

int main() {
    if (!stdio_init_all()) {
        printf("stdio init failed\n");
        return -1;
    }

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    uint baudrate = i2c_set_up(PICO_DEFAULT_I2C_INSTANCE, PICO_DEFAULT_I2C_SCL_PIN, PICO_DEFAULT_I2C_SDA_PIN, BAUDRATE);
    if (baudrate != BAUDRATE) {
        printf("baudrate set to %u instead of %u\n", baudrate, BAUDRATE);
    }

    //i2c_dev_t mpu6500 = {PICO_DEFAULT_I2C_INSTANCE, MPU_ADDRESS};
    i2c_dev_t bmp180 = {PICO_DEFAULT_I2C_INSTANCE, BMP180_ADDRESS};

    //mpuStart(mpu6500);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(5000);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    printf("Scan i2c\n");
    uint8_t addr[1 << 7];
    uint8_t nr = i2c_scan(PICO_DEFAULT_I2C_INSTANCE, addr);
    for (uint8_t i = 0; i < nr; i++) {
        printf("device: %u\n", addr[i]);
    }
    printf("\n");

    Barometer barometer = createBarometer(bmp180);

    /*
    mpuCalibrate(mpu6500);

    uint8_t data[14];

    double acceleration[3];
    double rotation[3];

    loop {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

        mpuCurrentData(mpu6500, data);

        for (uint8_t i = 0; i < 3; i++) {
            acceleration[i] = scale_lsb_2g * (int16_t)(((uint16_t)data[i * 2] << 8) | data[i * 2 + 1]);
        }

        for (uint8_t i = 0; i < 3; i++) {
            rotation[i] = scale_lsb_250dps * (int16_t)(((uint16_t)data[(i + 4) * 2] << 8) | data[(i + 4) * 2 + 1]);
        }

        printf("acc => x: %7.2lf\ty: %7.2lf\tz: %7.2lf\nrot => x: %7.2lf\ty: %7.2lf\tz: %7.2lf\n\n", acceleration[0], acceleration[1], acceleration[2], rotation[0], rotation[1], rotation[2]);

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(1000);
    }
    */

    double altitude, pressure, temperature;

    loop {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

        getMeasurementsBarometer(barometer, &pressure, &temperature);
        altitude = getAltitude(pressure);

        printf("bmp => a: %7.2lfm   p: %7.2lfPa   %7.2lfatm   t: %7.2lfK   %7.2lfdC\n\n", altitude, pressure, atm(pressure), temperature, dC(temperature));

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(1000);
    }
}

double getAltitude(double p) {
    return 44330.0 * (1 - pow((p / p0), 0.1902949572));
}

void mpuStart(i2c_dev_t device) {
    /* Reset device
     * DEVICE_RESET = 1 */
    i2c_set_register(device.i2c, device.addr, PWR_MGMT_1, 1 << H_RESET);
    sleep_ms(100);

    /* Wake up device and disable temperature
     * SLEEP = 0
     * PD_PTAT = 1 */
    i2c_set_register(device.i2c, device.addr, PWR_MGMT_1, 1 << PD_PTAT);
}

void mpuCalibrate(i2c_dev_t device) {
    uint8_t data[FIFO_SIZE];

    mpuConfigFIFO(device, true, false);
    printf("FIFO size: %u\n\n", mpuSizeFIFO(device));
    while(mpuSizeFIFO(device) < FIFO_SIZE);
    printf("FIFO size: %u\n\n", mpuSizeFIFO(device));
    mpuReadFIFO(device, data, FIFO_SIZE);
    mpuResetFIFO(device);

    int64_t gyro[3] = {0};

    for (uint8_t i = 0; i < FIFO_SIZE / 6; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            int16_t val = (int16_t)(((uint16_t)data[i * 6 + j] << 8) | data[i * 6 + j + 1]);
            gyro[j] += val;
        }
    }

    double dps[3] = {0};

    for (uint8_t i = 0; i < 3; i++) {
        gyro[i] /= FIFO_SIZE / 6;
        dps[i] = scale_lsb_250dps * (double)gyro[i];
    }

    printf("cal => x: %7.2lf\ty: %7.2lf\tz: %7.2lf\n\n", dps[0], dps[1], dps[2]);
}

void mpuConfigFIFO(i2c_dev_t device, bool gyro, bool accel) {
    uint8_t config = 0;

    if (gyro) {
        config |= 1 << GYRO_XOUT | 1 << GYRO_YOUT | 1 << GYRO_ZOUT;
    }

    if (accel) {
        config |= 1 << ACCEL;
    }

    mpuResetFIFO(device);

    i2c_set_register(device.i2c, device.addr, FIFO_EN_r, config);
}

void mpuResetFIFO(i2c_dev_t device) {
    uint8_t val;
    i2c_read_register(device.i2c, device.addr, USER_CTRL, &val);

    val |= 1 << FIFO_RST;
    i2c_set_register(device.i2c, device.addr, USER_CTRL, val);
}

void mpuCurrentData(i2c_dev_t device, void *data) {
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(device.i2c, device.addr, &reg, 1, true);
    i2c_read_blocking(device.i2c, device.addr, data, 14, false);
}

uint16_t mpuSizeFIFO(i2c_dev_t device) {
    uint8_t reg = FIFO_COUNTH;
    i2c_write_blocking(device.i2c, device.addr, &reg, 1, true);

    uint8_t data[2];
    i2c_read_blocking(device.i2c, device.addr, data, 2, false);

    return (((uint16_t)data[0]) << 8)  + data[1];
}

void mpuReadFIFO(i2c_dev_t device, void *data, uint16_t size) {
    uint8_t reg = FIFO_R_W;
    i2c_write_blocking(device.i2c, device.addr, &reg, 1, true);
    i2c_read_blocking(device.i2c, device.addr, data, size, false);
}

#pragma clang diagnostic pop