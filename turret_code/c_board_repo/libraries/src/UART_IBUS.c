/*
 * UART_IBUS.c
 *
 *  Created on: May 4, 2026
 *      Author: liangnie
 */

#include "main.h"
#ifdef INCLUDE_UART_IBUS
#include "UART_IBUS.h"
#include "string.h"
#include "UART_Core.h"

uint8_t IBUS_Buff[22];

extern UART_HandleTypeDef huart1;


UART_LibraryInfo UART_IBUS(){
	UART_LibraryInfo UART_IBUSInfo;
	UART_IBUSInfo.rx_passThrough_Fn = &UART_IBUS_Recieve;
	return UART_IBUSInfo;
}


void UART_IBUS_Recieve(UART_HandleTypeDef *huart, uint8_t rx_data[256]){
	if (huart != &huart1) {
		#ifdef UART_IBUS_DEBUG
			UART_Printf(UART1, "UART DID NOT PASS\r\n");
		#endif
		return;
	}
	#ifdef UART_IBUS_DEBUG
		UART_Printf(UART1, "Matched UART2!\r\n");
	#endif
	uint16_t header = rx_data[1]<<8 | rx_data[0];

	#ifdef UART_IBUS_DEBUG
		UART_Printf(UART1, "Header = %d!\r\n", header);
	#endif

	if (header != 16416) {
		#ifdef UART_IBUS_DEBUG
			UART_Printf(UART1, "NO HEADER MATCHED!\r\n");
		#endif
		return;
	}
	memcpy(IBUS_Buff,rx_data,22);


	#ifdef UART_IBUS_DEBUG
		UART_Printf(UART1, "HEADER MATCHED!\r\n");
	#endif
}

int16_t ScalerConversion(int16_t FoundValue, uint8_t JoystickOrSwitch) {
	if (FoundValue == 0) {
		if (JoystickOrSwitch == IBUS_Switch) {
			return -660;
		}
		return 0;
	}
	float temp = (float)(FoundValue-1500);
	return (int16_t)(temp*1.32f);
}

int16_t getIBUSCH1() {
	int16_t FoundValueTemp = IBUS_Buff[3]<<8 | IBUS_Buff[2];
	// UART_Printf(UART1, "%d\r\n", FoundValueTemp);
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Joystick);
	return FoundValue;
}

int16_t getIBUSCH2() {
	int16_t FoundValueTemp = IBUS_Buff[5]<<8 | IBUS_Buff[4];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Joystick);
	return FoundValue;
}

int16_t getIBUSCH3() {
	int16_t FoundValueTemp = IBUS_Buff[7]<<8 | IBUS_Buff[6];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Joystick);
	return FoundValue;
}

int16_t getIBUSCH4() {
	int16_t FoundValueTemp = IBUS_Buff[9]<<8 | IBUS_Buff[8];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Joystick);
	return FoundValue;
}

int16_t getIBUSCH5() {
	int16_t FoundValueTemp = IBUS_Buff[11]<<8 | IBUS_Buff[10];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}

int16_t getIBUSCH6() {
	int16_t FoundValueTemp = IBUS_Buff[13]<<8 | IBUS_Buff[12];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}

int16_t getIBUSCH7() {
	int16_t FoundValueTemp = IBUS_Buff[15]<<8 | IBUS_Buff[14];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}

int16_t getIBUSCH8() {
	int16_t FoundValueTemp = IBUS_Buff[17]<<8 | IBUS_Buff[16];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}

int16_t getIBUSCH9() {
	int16_t FoundValueTemp = IBUS_Buff[19]<<8 | IBUS_Buff[18];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}

int16_t getIBUSCH10() {
	int16_t FoundValueTemp = IBUS_Buff[21]<<8 | IBUS_Buff[20];
	int16_t FoundValue = ScalerConversion(FoundValueTemp, IBUS_Switch);
	return FoundValue;
}
#endif
