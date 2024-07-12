#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef BMP180_H
#define BMP180_H

#define BMP180_ADDRESS  0x77

/* https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf */

#define TemperatureMeasurement 0x2E
#define PressureMeasurement(oss) (0x34 + ((oss) << 6))

/* Data registers */
#define out_xlsb 248
#define out_lsb 247
#define out_msb 246

/* Control registers */
#define ctrl_meas 244
#define softReset 224

/* Fixed */
#define id_r 208

/* Calibration registers */
#define calib21 191
#define calib20 190
#define calib19 189
#define calib18 188
#define calib17 187
#define calib16 186
#define calib15 185
#define calib14 184
#define calib13 183
#define calib12 182
#define calib11 181
#define calib10 180
#define calib9 179
#define calib8 178
#define calib7 177
#define calib6 176
#define calib5 175
#define calib4 174
#define calib3 173
#define calib2 172
#define calib1 171
#define calib0 170

#endif /* BMP180_H */

#pragma clang diagnostic pop