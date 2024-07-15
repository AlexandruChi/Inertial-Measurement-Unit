#ifndef BAROMETER_H
#define BAROMETER_H

#include "i2c/i2c.h"

typedef void* Barometer;

__attribute__((unused)) Barometer createBarometer(i2c_dev_t device);
__attribute__((unused)) void getMeasurementsBarometer(Barometer barometer, double *pressure, double *temperature);
__attribute__((unused)) void destroyBarometer(Barometer *barometer);

#endif /* BAROMETER_H */