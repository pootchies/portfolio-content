/*
 * MPU6050.c
 *
 *  Created on: May 12, 2026
 *      Author: liangnie
 */

#include "main.h"
#ifdef INCLUDE_MPU6050

#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c2;

const uint8_t MPU_6050_ADDRESS = 0xD0;

// INDEX 0 = Accelerometer, INDEX 1 = Gyro
MPU6050_SenseStore MPU_6050_SENS[2] = {{.dataPackage = 0, .scaler = 1670.13f}, {.dataPackage = 0, .scaler = 131.0f}};
MPU6050_Storage MPU_6050_DATA = {.accelCalibrated = {0}, .gyroCalibrated = {0}, .tempCalibrated = 0, .accelRaw = {0}, .gyroRaw = {0}, .tempRaw = 0};

uint8_t GYRO_SENS_TX = 0x00;
uint8_t ACCEL_SENS_TX = 0x00;



/*	Sets the sensitivity of the ACCELEROMETER for the IMU
 *
 * 	The available values to set are the following:
 * 		0 = ±2g
 * 		1 = ±4g
 * 		2 = ±8g
 * 		3 = ±16g
 *
 * 	Recommended Value = ±16g = 3
 */
void MPU6050_ACCEL_SENS(uint8_t fullScaleSensitivity) {

	ACCEL_SENS_TX = fullScaleSensitivity << 3;
	MPU_6050_SENS[0].dataPackage = fullScaleSensitivity;
	switch (fullScaleSensitivity) {
	case 0:
		MPU_6050_SENS[0].scaler = 1670.13f;
		break;
	case 1:
		MPU_6050_SENS[0].scaler = 835.066f;
		break;
	case 2:
		MPU_6050_SENS[0].scaler = 417.5331f;
		break;
	case 3:
	default:
		MPU_6050_SENS[0].scaler = 208.7666f;
		break;
	};

}

/*	Sets the sensitivity of the GYRO for the IMU
 *
 * 	The available values to set are the following:
 * 		0 = ±250°/s
 * 		1 = ±500°/s
 * 		2 = ±1000°/s
 * 		3 = ±2000°/s
 *
 * 	Recommended Value = ±2000°/s = 3
 */
void MPU6050_GYRO_SENS(uint8_t fullScaleSensitivity) {
	GYRO_SENS_TX = fullScaleSensitivity << 3;
	MPU_6050_SENS[1].dataPackage = fullScaleSensitivity;
	switch (fullScaleSensitivity) {
	case 0:
		MPU_6050_SENS[1].scaler = 131.0f;
		break;
	case 1:
		MPU_6050_SENS[1].scaler = 65.5f;
		break;
	case 2:
		MPU_6050_SENS[1].scaler = 32.8f;
		break;
	case 3:
	default:
		MPU_6050_SENS[1].scaler = 16.4f;
		break;
	};
}

/*
void _RESET_NVIC_LMAO() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
	UART_Printf(UART2, "KILL ITSELF!!!!\r\n");
	osDelay(100);
	NVIC_SystemReset();
}
*/

void _MPU6050_FunctionReadChecker(HAL_StatusTypeDef (*inputFunction)(I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t), uint16_t MemAddress, uint8_t *pData, uint8_t pDataSize) {

	switch (inputFunction(&hi2c2, MPU_6050_ADDRESS, MemAddress, 1, pData, pDataSize, 10)) {

	case 1:
		#ifdef INCLUDE_MPU6050_DEBUG
			UART_Printf(UART2, "Read HAL_ERROR\r\n");
		#endif
		// _RESET_NVIC_LMAO();
		return;
	case 2:
		#ifdef INCLUDE_MPU6050_DEBUG
			UART_Printf(UART2, "Read HAL_BUSY\r\n");
		#endif
		// _RESET_NVIC_LMAO();
		return;

	default:
	}

	return;
}

void _MPU6050_FunctionWriteChecker(HAL_StatusTypeDef (*inputFunction)(I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t), uint16_t MemAddress, uint8_t *pData) {

	switch (inputFunction(&hi2c2, MPU_6050_ADDRESS, MemAddress, 1, pData, 1, 10)) {

	case 1:
		#ifdef INCLUDE_MPU6050_DEBUG
			UART_Printf(UART2, "Write HAL_ERROR\r\n");
		#endif
		// _RESET_NVIC_LMAO();
		break;
	case 2:
		#ifdef INCLUDE_MPU6050_DEBUG
			UART_Printf(UART2, "Write HAL_BUSY\r\n");
		#endif
		// _RESET_NVIC_LMAO();
		break;

	default:
	}

	uint8_t checkValue = 0;
	_MPU6050_FunctionReadChecker(HAL_I2C_Mem_Read, MemAddress ,&checkValue, 1);

	if (*pData != checkValue) {
		#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "Written Value did NOT MATCH!!! Written Value = %d, Checked Value = %d\r\n", *pData, checkValue);
		#endif
	} else {
		#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "MATCH!!! Written Value = %d, Checked Value = %d\r\n", *pData, checkValue);
		#endif
	}


}

/*
 *
 */
void MPU6050_INIT() {

	/*HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
	osDelay(10);
*/
	uint8_t RXB1 = 0;
	uint8_t TXMessage = 0;

	// Checks the data for the WHO AM I to see if the values I'm receiving are good
	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x75 Mem Read ------\r\n");
	#endif
	_MPU6050_FunctionReadChecker(HAL_I2C_Mem_Read, 0x75, &RXB1, 1);

	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "WHO AM I = %d\r\n", RXB1);
	#endif

	if (RXB1 != 104) {
		#ifdef INCLUDE_MPU6050_DEBUG
			UART_Printf(UART2, "WHO AM I did not match\r\n");
		#endif
	}

	TXMessage = 0x00;
	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x6B Mem Write ------\r\n");
	#endif
	 _MPU6050_FunctionWriteChecker(HAL_I2C_Mem_Write, 0x6B, &TXMessage);

	TXMessage = 0x07;
	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x19 Mem Write ------\r\n");
	#endif
	 _MPU6050_FunctionWriteChecker(HAL_I2C_Mem_Write, 0x19, &TXMessage);

	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x1B Mem Write ------\r\n");
	#endif

	 _MPU6050_FunctionWriteChecker(HAL_I2C_Mem_Write, 0x1B, &GYRO_SENS_TX);
	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x1C Mem Write ------\r\n");
	#endif
	 _MPU6050_FunctionWriteChecker(HAL_I2C_Mem_Write, 0x1C, &ACCEL_SENS_TX);
}

void MPU6050_Update() {
	uint8_t rxBuff[14] = {0};
	// HAL_I2C_Mem_Read(&hi2c2, MPU_6050_ADDRESS, 0x3B, 1, rxBuff, 14, 1000);

	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "\r\n------ Starting 0x3B Mem Read ------\r\n");
	#endif
	_MPU6050_FunctionReadChecker(HAL_I2C_Mem_Read, 0x3B, rxBuff, 14);
	/*
	#ifdef INCLUDE_MPU6050_DEBUG
		UART_Printf(UART2, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", rxBuff[0], rxBuff[1], rxBuff[2], rxBuff[3], rxBuff[4], rxBuff[5], rxBuff[6], rxBuff[7], rxBuff[8], rxBuff[9], rxBuff[10], rxBuff[11], rxBuff[12], rxBuff[13]);
	#endif
	*/

	for (uint8_t i = 0; i < 3; i++){
		MPU_6050_DATA.accelRaw[i] = (int16_t)((rxBuff[i*2]<<8) | rxBuff[(i*2)+1]);
		MPU_6050_DATA.gyroRaw[i] = (int16_t)((rxBuff[(i+4)*2]<<8) | rxBuff[((i+4)*2)+1]);
	}
	MPU_6050_DATA.tempRaw = (int16_t)((rxBuff[6]<<8) | rxBuff[7]);

	for (uint8_t i = 0; i < 3; i++) {
		MPU_6050_DATA.accelCalibrated[i] = MPU_6050_DATA.accelRaw[i]/MPU_6050_SENS[0].scaler;
		MPU_6050_DATA.gyroCalibrated[i] = MPU_6050_DATA.gyroRaw[i]/MPU_6050_SENS[1].scaler;
	}
	MPU_6050_DATA.tempCalibrated = (MPU_6050_DATA.tempRaw/340)+36.53;


	#ifdef INCLUDE_MPU6050_DEBUG
		// UART_Printf(UART2, "%f, %f, %f\r\n", MPU_6050_DATA.accelCalibrated[0], MPU_6050_DATA.accelCalibrated[1], MPU_6050_DATA.accelCalibrated[2]);
		// UART_Printf(UART2, "%f, %f, %f\r\n", MPU_6050_DATA.gyroCalibrated[0], MPU_6050_DATA.gyroCalibrated[1], MPU_6050_DATA.gyroCalibrated[2]);
		// UART_Printf(UART2, "%f\r\n", MPU_6050_DATA.tempCalibrated);
	#endif

}

float MPU6050_get_gyro_x() {
	return MPU_6050_DATA.gyroCalibrated[0];
}
float MPU6050_get_gyro_y() {
	return MPU_6050_DATA.gyroCalibrated[1];
}
float MPU6050_get_gyro_z() {
	return MPU_6050_DATA.gyroCalibrated[2];
}
float MPU6050_get_accel_x() {
	return MPU_6050_DATA.accelCalibrated[0];
}
float MPU6050_get_accel_y() {
	return MPU_6050_DATA.accelCalibrated[1];
}
float MPU6050_get_accel_z() {
	return MPU_6050_DATA.accelCalibrated[2];
}
float MPU6050_get_temp() {
	return MPU_6050_DATA.tempCalibrated;
}

#endif
