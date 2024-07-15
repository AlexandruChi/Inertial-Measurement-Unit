#ifndef BAROMETER_H
#define BAROMETER_H

// For BMP180

#include "i2c/i2c.h"
#include "units.h"

#define p0 hPa(1013.25) // Sea level pressure

typedef void* Barometer;

__attribute__((unused)) Barometer createBarometer(i2c_dev_t device);
__attribute__((unused)) bool getMeasurementsBarometer(Barometer barometer, double *pressure, double *temperature);
__attribute__((unused)) void destroyBarometer(Barometer *barometer);

#endif /* BAROMETER_H */