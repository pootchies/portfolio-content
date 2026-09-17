/*
 * DBUS.c
 *
 *  Created on: May 13, 2025
 *      Author: moose
 */

#include "main.h"
#ifdef INCLUDE_DBUS_DR16
#include "DBUS_DR16.h"
#include "string.h"
#include "UART_Core.h"


DR16_DBUS_t DR16_DBUS;
uint8_t DBUS_Buff[32];

uint8_t DBUS_getBits(uint8_t data, uint8_t startBit, uint8_t len);

UART_HandleTypeDef huart3 __attribute__((weak));
extern UART_HandleTypeDef huart3;

UART_LibraryInfo DBUS_DR16(){
	UART_LibraryInfo DBUS_DR16Info;
	DBUS_DR16Info.rx_passThrough_Fn = &DBUS_DR16_Recieve;
	UART_Ex_Init(&huart3);
	return DBUS_DR16Info;
}

void DBUS_DR16_Recieve(UART_HandleTypeDef *huart, uint8_t rx_data[256]){
	if(huart==&huart3){
		memcpy(&DR16_DBUS,rx_data,sizeof(DR16_DBUS_t));
	}
}

int16_t getDR16_CH0(){

	return DR16_DBUS.CH0>0 ? DR16_DBUS.CH0-1024: 0;
}

int16_t getDR16_CH1(){

	return DR16_DBUS.CH1>0 ? DR16_DBUS.CH1-1024: 0;
}

int16_t getDR16_CH2(){

	return DR16_DBUS.CH2>0 ? DR16_DBUS.CH2-1024: 0;
}

int16_t getDR16_CH3(){
	return DR16_DBUS.CH3>0 ? DR16_DBUS.CH3-1024: 0;
}

int16_t getDR16_CH4(){
	return DR16_DBUS.RESERVED>0 ? DR16_DBUS.RESERVED-1024: 0;
}

uint8_t getDR16_S1(){
	return DR16_DBUS.S1;
}

uint8_t getDR16_S2(){
	return DR16_DBUS.S2;
}

uint8_t getDR16_S1UP(){
	return DR16_DBUS.S1==1;
}

uint8_t getDR16_S1MID(){
	return DR16_DBUS.S1==3;
}

uint8_t getDR16_S1DWN(){
	return DR16_DBUS.S1==2;
}

uint8_t getDR16_S2UP(){
	return DR16_DBUS.S2==1;
}

uint8_t getDR16_S2MID(){
	return DR16_DBUS.S2==3;
}

uint8_t getDR16_S2DWN(){
	return DR16_DBUS.S2==2;
}

uint8_t getDR16_W(){
	return DR16_DBUS.w;
}

uint8_t getDR16_S(){
	return DR16_DBUS.s;
}

uint8_t getDR16_A(){
	return DR16_DBUS.a;
}

uint8_t getDR16_D(){
	return DR16_DBUS.d;
}

uint8_t getDR16_Q(){
	return DR16_DBUS.q;
}

uint8_t getDR16_E(){
	return DR16_DBUS.e;
}

uint8_t getDR16_Shift(){
	return DR16_DBUS.shift;
}

uint8_t getDR16_Ctrl(){
	return DR16_DBUS.ctrl;
}

uint8_t getDR16_R(){
	return DR16_DBUS.r;
}

uint8_t getDR16_F(){
	return DR16_DBUS.f;
}

uint8_t getDR16_G(){
	return DR16_DBUS.g;
}

uint8_t getDR16_Z(){
	return DR16_DBUS.z;
}

uint8_t getDR16_X(){
	return DR16_DBUS.x;
}

uint8_t getDR16_C(){
	return DR16_DBUS.c;
}

uint8_t getDR16_V(){
	return DR16_DBUS.v;
}

uint8_t getDR16_B(){
	return DR16_DBUS.b;
}

int16_t getDR16_MouseX(){

	return DR16_DBUS.MouseX;
}

int16_t getDR16_MouseY(){
	return DR16_DBUS.MouseY;
}

int16_t getDR16_MouseZ(){
	return DR16_DBUS.MouseZ;
}

uint8_t getDR16_MouseR(){
	return DR16_DBUS.MouseR;
}

uint8_t getDR16_MouseL(){
	return DR16_DBUS.MouseL;
}

#endif

