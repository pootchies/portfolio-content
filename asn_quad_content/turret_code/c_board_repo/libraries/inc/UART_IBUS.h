/*
 * UART_IBUS.h
 *
 *  Created on: May 4, 2026
 *      Author: liangnie
 */
#include "main.h"
#ifdef INCLUDE_UART_IBUS

#ifndef INC_UART_IBUS_H_
#define INC_UART_IBUS_H_

UART_LibraryInfo UART_IBUS();
void UART_IBUS_Recieve(UART_HandleTypeDef *huart, uint8_t rx_data[256]);

typedef enum {
	IBUS_Switch = 0,
	IBUS_Joystick = 1,
}IBUS_t;

int16_t getIBUSCH1();
int16_t getIBUSCH2();
int16_t getIBUSCH3();
int16_t getIBUSCH4();
int16_t getIBUSCH5();
int16_t getIBUSCH6();
int16_t getIBUSCH7();
int16_t getIBUSCH8();
int16_t getIBUSCH9();
int16_t getIBUSCH10();

#endif
#endif /* INC_UART_IBUS_H_ */
