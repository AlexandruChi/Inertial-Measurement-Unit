#include <malloc.h>
#include "Barometer.h"
#include "BMP180/BMP180.h"

struct barometer_s {
    i2c_dev_t device;
    struct BMP180 *bmp180;
};

__attribute__((unused)) Barometer createBarometer(i2c_dev_t device) {
    struct barometer_s *barometer = malloc(sizeof(struct barometer_s));
    barometer->device = device;
    if (!(barometer->bmp180 = BMP180SetUp(device))) {
        free(barometer);
        return NULL;
    }
    return barometer;
}

__attribute__((unused)) bool getMeasurementsBarometer(Barometer barometer, double *pressure, double *temperature) {
    long T, p;
    BMP180Measure(((struct barometer_s *) barometer)->device, *((struct barometer_s *) barometer)->bmp180, 3, &T, &p);
    *pressure = (double)p;
    *temperature = 0.1 * T + 273.15;
}

__attribute__((unused)) void destroyBarometer(Barometer *barometer) {
    if (*barometer) {
        if (((struct barometer_s*)(*barometer))->bmp180) {
            BMP180Clear(&((struct barometer_s*)(*barometer))->bmp180);
        }
        free(*barometer);
        *barometer = NULL;
    }
}