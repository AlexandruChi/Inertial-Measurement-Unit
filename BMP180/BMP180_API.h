#ifndef BMP180_API_H
#define BMP180_API_H

#include "i2c/i2c.h"

struct BMP180 {
    short AC1;
    short AC2;
    short AC3;
    unsigned short AC4;
    unsigned short AC5;
    unsigned short AC6;
    short B1;
    short B2;
    short MB;
    short MC;
    short MD;
};

struct BMP180 *BMP180SetUp(i2c_dev_t device);
bool BMPMeasure(i2c_dev_t device, struct BMP180 calibration, uint8_t oversampling, long *T, long *p);
void BMP180Clear(struct BMP180 **calibration);

#endif /* BMP180_API_H */
