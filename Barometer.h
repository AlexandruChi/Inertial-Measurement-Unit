#ifndef BAROMETER_H
#define BAROMETER_H

#include "i2c/i2c.h"

typedef void* Barometer;

Barometer createBarometer(i2c_dev_rw_t device);
void getMeasurementsBarometer(Barometer barometer, double *pressure, double *temperature);
void destroyBarometer(Barometer *barometer);

#endif /* BAROMETER_H */