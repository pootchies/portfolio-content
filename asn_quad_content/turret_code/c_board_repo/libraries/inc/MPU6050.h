/*
 * MPU6050.h
 *
 *  Created on: May 12, 2026
 *      Author: liangnie
 */

#include "main.h"
#ifdef INCLUDE_MPU6050
#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

/*
typedef enum {

};
*/

typedef struct {
	uint8_t dataPackage;
	float scaler;
}MPU6050_SenseStore;

typedef struct {
	int16_t gyroRaw[3];
	int16_t accelRaw[3];
	int16_t tempRaw;
	uint32_t sum;
	float gyroCalibrated[3];
	float accelCalibrated[3];
	float tempCalibrated;
}MPU6050_Storage;

void MPU6050_GYRO_SENS(uint8_t fullScaleSensitivity);
void MPU6050_ACCEL_SENS(uint8_t fullScaleSensitivity);

void MPU6050_INIT();
void MPU6050_Update();

float MPU6050_get_gyro_x();
float MPU6050_get_gyro_y();
float MPU6050_get_gyro_z();
float MPU6050_get_accel_x();
float MPU6050_get_accel_y();
float MPU6050_get_accel_z();
float MPU6050_get_temp();

#endif /* INC_MPU6050_H_ */
#endif
