#include "BMP180.h"
#include "BMP180_API.h"

#include <malloc.h>

struct BMP180 *BMP180SetUp(i2c_dev_rw_t device) {
    uint8_t reg = calib0;
    i2c_write_blocking(device.i2c, device.addr_w, &reg, 1, true);

    uint8_t data[22];
    i2c_read_blocking(device.i2c, device.addr_r, data, 22, false);

    uint16_t *calibration = malloc(sizeof(struct BMP180));
    for (uint8_t i = 0; i < 11; i++) {
        calibration[i] = ((uint16_t)data[i * 2] << 8) | data[i * 2 + 1];
        if (calibration[i] == 0 || calibration[i] == (uint16_t)-1) {
            free(calibration);
            return NULL;
        }
    }

    return (struct BMP180*)calibration;
}

bool BMPMeasure(i2c_dev_rw_t device, struct BMP180 calibration, uint8_t oversampling, long *T, long *p) {
    const uint16_t waitTime[4] = {4500, 7500, 13500, 25500};

    if (oversampling > 3) {
        return false;
    }

    /* read uncompensated temperature value */

    i2c_set_register(device.i2c, device.addr_w, ctrl_meas, TemperatureMeasurement);
    sleep_us(4500);

    uint8_t reg = out_msb;
    uint8_t data[3];

    i2c_write_blocking(device.i2c, device.addr_w, &reg, 1, true);
    i2c_read_blocking(device.i2c, device.addr_r, data, 2, false);

    long UT = ((uint16_t)data[0] << 8) | data[1];

    /* Read uncompensated pressure value */

    i2c_set_register(device.i2c, device.addr_w, ctrl_meas, PressureMeasurement(oversampling));
    sleep_us(waitTime[oversampling]);

    i2c_write_blocking(device.i2c, device.addr_w, &reg, 1, true);
    i2c_read_blocking(device.i2c, device.addr_r, data, 3, false);

    long UP = (long)((((uint32_t)data[0] << 16) | ((uint16_t)data[1] << 8) | data[2]) >> (8 - oversampling));

    /* Calculate true temperature */

    long X1 = (UT - calibration.AC6) * calibration.AC5 / (2 << 15);
    long X2 = calibration.MC * (2 << 11) / (X1 + calibration.MD);
    long B5 = X1 + X2;

    *T = (B5 + 8) / (2 << 4); // in 0.1 dC

    /* Calculate true pressure */

    long B6 = B5 - 4000;
    X1 = (calibration.B2 * (B6 * B6 / (2 << 12))) / (2 << 11);
    X2 = calibration.AC2 * B6 / (2 << 11);
    long X3 = X1 + X2;
    long B3 = (((calibration.AC1 * 4 + X3) << oversampling) + 2) / 4;
    X1 = calibration.AC3 * B6 / (2 << 13);
    X2 = (calibration.B1 * (B6 * B6 / (2 << 12))) / (2 << 16);
    X3 = ((X1 + X2) + 2) / (2 << 2);
    unsigned long B4 = calibration.AC4 * (unsigned long)(X3 + 32768) / (2 << 15);
    unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oversampling);
    if (B7 < 0x80000000) {
        *p = (long)((B7 * 2) / B4);
    } else {
        *p = (long)((B7 / B4) * 2);
    }
    X1 = (*p / (2 << 8)) * (*p / (2 << 8));
    X1 = (X1 * 3038) / (2 << 16);
    X2 = (-7357 * *p) / (2 << 16);
    *p = *p + (X1 + X2 + 3791) / (2 << 4); // in Pa

    return true;
}

void BMP180Clear(struct BMP180 **calibration) {
    if (*calibration) {
        free(*calibration);
        *calibration = NULL;
    }
}