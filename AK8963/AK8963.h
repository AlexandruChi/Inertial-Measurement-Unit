#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef AK8963_H
#define AK8963_H

#define AK8963_ADDRESS

/* https://invensense.tdk.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf */

/* Device ID */
#define WIA 0

/* Information */
#define INFO 1

/* Status 1  */
#define ST1 2

/* Measurement data */
#define HXL 3
#define HXH 4
#define HYL 5
#define HYH 6
#define HZL 7
#define HZH 8

/* Status 2 */
#define ST2 9

/* Control */
#define CNTL 10

/* Self-test */
#define ASTC 12

/* I2C disable */
#define I2CDIS 15

#endif /* AK8963_H */

#pragma clang diagnostic pop