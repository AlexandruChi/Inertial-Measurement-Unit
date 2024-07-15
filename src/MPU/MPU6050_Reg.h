#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

/* MPU */
#ifndef MPU6050_Reg_H
#define MPU6050_Reg_H

#define MPU_ADDRESS     0x68 /* INT = GND */
#define MPU_ADDRESS_2   0x69 /* INT = VDD */

/* Self Test Registers */
#define SELF_TEST_X 13
#define SELF_TEST_Y 14
#define SELF_TEST_Z 15
#define SELF_TEST_A 16

/* Sample Rate Divider */
#define SMPLRT_DIV 25

/* Configuration */
#define CONFIG 26

/* Gyroscope Configuration */
#define GYRO_CONFIG 27

/* Accelerometer Configuration */
#define ACCEL_CONFIG 28

/* FIFO Enable */
#define FIFO_EN 35

/* I2C Master Control */
#define I2C_MST_CTRL 36

/* I2C Slave 0 Control */
#define I2C_SLV0_ADDR 37
#define I2C_SLV0_REG 38
#define I2C_SLV0_CTRL 39

/* I2C Slave 1 Control */
#define I2C_SLV1_ADDR 40
#define I2C_SLV1_REG 41
#define I2C_SLV1_CTRL 42

/* I2C Slave 2 Control */
#define I2C_SLV2_ADDR 43
#define I2C_SLV2_REG 44
#define I2C_SLV2_CTRL 45

/* I2C Slave 3 Control */
#define I2C_SLV3_ADDR 46
#define I2C_SLV3_REG 47
#define I2C_SLV3_CTRL 48

/* I2C Slave 4 Control */
#define I2C_SLV4_ADDR 49
#define I2C_SLV4_REG 50
#define I2C_SLV4_DO 51
#define I2C_SLV4_CTRL 52
#define I2C_SLV4_DI 53

/* I2C Master Status */
#define I2C_MST_STATUS 54

/* INT Pin / Bypass Enable Configuration */
#define INT_PIN_CFG 55

// Parameters position
#define INT_LEVEL 7
#define INT_OPEN 6
#define LATCH_INT_EN 5
#define INT_RD_CLEAR 4
#define FSYNC_INT_LEVEL 3
#define FSYNC_INT_EN 2
#define I2C_BYPASS_EN 1

/* Interrupt Enable */
#define INT_ENABLE 56

// Parameters position
#define FIFO_OFLOW_EN 4
#define I2C_MST_INT_EN 3
#define DATA_RDY_EN 0

/* Interrupt Status */
#define INT_STATUS 58

// Parameters position
#define FIFO_OFLOW_INT 4
#define I2C_MST_INT 3
#define DATA_RDY_INT 0

/* Accelerometer Measurements */
#define ACCEL_XOUT_H 59
#define ACCEL_XOUT_L 60
#define ACCEL_YOUT_H 61
#define ACCEL_YOUT_L 62
#define ACCEL_ZOUT_H 63
#define ACCEL_ZOUT_L 64

/* Temperature Measurement */
#define TEMP_OUT_H 65
#define TEMP_OUT_L 66

/* Gyroscope Measurements */
#define GYRO_XOUT_H 67
#define GYRO_XOUT_L 68
#define GYRO_YOUT_H 69
#define GYRO_YOUT_L 70
#define GYRO_ZOUT_H 71
#define GYRO_ZOUT_L 72

/* External Sensor Data */
#define EXT_SENS_DATA_00 73
#define EXT_SENS_DATA_02 75
#define EXT_SENS_DATA_03 76
#define EXT_SENS_DATA_04 77
#define EXT_SENS_DATA_05 78
#define EXT_SENS_DATA_06 79
#define EXT_SENS_DATA_07 80
#define EXT_SENS_DATA_08 81
#define EXT_SENS_DATA_09 82
#define EXT_SENS_DATA_10 83
#define EXT_SENS_DATA_11 84
#define EXT_SENS_DATA_12 85
#define EXT_SENS_DATA_13 86
#define EXT_SENS_DATA_14 87
#define EXT_SENS_DATA_15 88
#define EXT_SENS_DATA_16 89
#define EXT_SENS_DATA_17 90
#define EXT_SENS_DATA_18 91
#define EXT_SENS_DATA_19 92
#define EXT_SENS_DATA_20 93
#define EXT_SENS_DATA_21 94
#define EXT_SENS_DATA_22 95
#define EXT_SENS_DATA_23 96

/* Slave 0 Data Out */
#define I2C_SLV0_DO 99

/* I2C Slave 1 Data Out */
#define I2C_SLV1_DO 100

/* I2C Slave 2 Data Out */
#define I2C_SLV2_DO 101

/* I2C Slave 3 Data Out */
#define I2C_SLV3_DO 102

/* I2C Master Delay Control */
#define I2C_MST_DELAY_CTRL 103

/* Signal Path Reset */
#define SIGNAL_PATH_RESET 104

/* User Control */
#define USER_CTRL 106

/* Power Management 1 */
#define PWR_MGMT_1 107

// Parameters position
#define DEVICE_RESET 7
#define SLEEP 6
#define CYCLE 5
#define TEMP_DIS 3
#define CLKSEL 0

/* Power Management 2 */
#define PWR_MGMT_2 108

/* FIFO Count Registers */
#define FIFO_COUNTH 114
#define FIFO_COUNTL 115

/* FIFO Read Write */
#define FIFO_R_W 116

/* Who Am I */
#define WHO_AM_I 117

#endif /* MPU6050_Reg_H */

#pragma clang diagnostic pop