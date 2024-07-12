#include <malloc.h>
#include <pico/printf.h>
#include "Barometer.h"
#include "BMP180/BMP180_API.h"

struct barometer_s {
    i2c_dev_t device;
    struct BMP180 *bmp180;
};

Barometer createBarometer(i2c_dev_t device) {
    struct barometer_s *barometer = malloc(sizeof(struct barometer_s));
    barometer->device = device;
    if (!(barometer->bmp180 = BMP180SetUp(device))) {
        free(barometer);
        printf("Error reading calibration data\n");
        return NULL;
    }
    return barometer;
}

void getMeasurementsBarometer(Barometer barometer, double *pressure, double *temperature) {
    long T, p;
    BMPMeasure(((struct barometer_s*)barometer)->device, *((struct barometer_s*)barometer)->bmp180, 3, &T, &p);
    *pressure = (double)p;
    *temperature = 0.1 * T + 273.15;
}

void destroyBarometer(Barometer *barometer) {
    if (*barometer) {
        if (((struct barometer_s*)(*barometer))->bmp180) {
            BMP180Clear(&((struct barometer_s*)(*barometer))->bmp180);
        }
        free(*barometer);
        *barometer = NULL;
    }
}