/*
 * UART_Core.c
 *
 *  Created on: Apr 7, 2026
 *      Author: Gen6
 */

#include "main.h"
#ifdef INCLUDE_UART_CORE
#include "UART_Core.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"


UART_LibraryInfo* UART_Libraries = NULL;
uint8_t UART_Libraries_Count = 0;

UART_Instance_t* listOfUarts = NULL;
uint8_t countOfUarts = 0;



static uint8_t rxInitBuff[256] = {0};


void _UART_LinkLib(int n_arg, ...){
	va_list libraries;
	va_start(libraries,n_arg);
	if(UART_Libraries_Count!=0){
		free(UART_Libraries);
	}
	UART_Libraries = calloc(n_arg,sizeof(UART_LibraryInfo));
	UART_Libraries_Count = n_arg;
	if(UART_Libraries==NULL){
		UART_Libraries_Count = 0;
	}
	for(int i = 0; i<n_arg;i++){
		UART_Libraries[i] = va_arg(libraries,UART_LibraryInfo);
	}
	va_end(libraries);
}


UART_HandleTypeDef huart1 __attribute__((weak));
UART_HandleTypeDef huart6 __attribute__((weak));

/*
** Descriptions: Wrapper for c_board, Initializes huart6 as 1 and huart 1 as 2
*/
void UART_Init() {
	extern UART_HandleTypeDef huart1;
	extern UART_HandleTypeDef huart6;
	extern UART_HandleTypeDef huart3;

	UART_Ex_Init(&huart6);
	UART_Ex_Init(&huart1);
}

/*
** Descriptions: Initialize the chosen uart instance using advanced reception service.
** 				 Must be called previously to enable HAL_UARTEx_RxEventCallback function
** Input: Target uart instance
** Return: Returns the uart number that this instance has been assigned. 220 if memory error
*/
uint8_t UART_Ex_Init(UART_HandleTypeDef *huart) {

	if(!countOfUarts){

		//create an array of uart objects
		listOfUarts = (UART_Instance_t*) malloc(sizeof(UART_Instance_t));

	}
	else{
		UART_Instance_t *temp = realloc(listOfUarts,(countOfUarts+1)*sizeof(UART_Instance_t));
		if(temp){
			listOfUarts = temp;
		}else{
			free(temp);
			return 220;
		}
	}

	countOfUarts++;

	listOfUarts[countOfUarts-1].huartx = huart;
	memset(listOfUarts[countOfUarts-1].rxBuff, 0, sizeof(listOfUarts[countOfUarts-1].rxBuff));

	HAL_UARTEx_ReceiveToIdle_DMA(huart, rxInitBuff, 256);

	return countOfUarts;
}


/*
** Descriptions: is triggered on recieve
** Input:
** Output:
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {

	uint8_t uartInstance = 0;

	for(uint8_t i = 0;i<countOfUarts;i++){
		if(huart==listOfUarts[i].huartx){
			uartInstance = i+1;
			break;
		}
	}

	//write uart to RxBuff until idle state
	if(uartInstance == 0){
		return;
	}

	HAL_UARTEx_ReceiveToIdle_DMA(huart, listOfUarts[uartInstance-1].rxBuff, 256);

	for(uint8_t i = 0; i<UART_Libraries_Count;i++){
		UART_Libraries[i].rx_passThrough_Fn(huart,listOfUarts[uartInstance-1].rxBuff);
	}

}

/*
** Descriptions: Send a message in fmt format over chosen uart instance
** Input: UART number to send over, message to send in fmt format
** Output: message transmitted over uart
*/
void _UART_Printf_Num(uint8_t UART_Number,const char *fmt,...) {

	if(UART_Number>countOfUarts){
		return;
	}
	UART_HandleTypeDef *huart = listOfUarts[UART_Number-1].huartx;
	static uint8_t tx_buf[256] = {0};
    static va_list ap;
    static uint16_t len;
    va_start(ap, fmt);
    len = vsprintf((char *)tx_buf, fmt, ap);
    va_end(ap);
    HAL_UART_Transmit(huart, tx_buf, len,100);
}

/*
** Descriptions: Send a message in fmt format over chosen uart instance
** Input: UART instance to send over, message to send in fmt format
** Output: message transmitted over uart
*/
void _UART_Printf_Inst(UART_HandleTypeDef *huart,const char *fmt,...) {
    static uint8_t tx_buf[256] = {0};
    static va_list ap;
    static uint16_t len;
    va_start(ap, fmt);
    len = vsprintf((char *)tx_buf, fmt, ap);
    va_end(ap);
    HAL_UART_Transmit(huart, tx_buf, len,100);
}

uint8_t* _UART_Scanf_Num(uint8_t UART_Number){
	if(UART_Number>countOfUarts){
		return 0;
	}
	return listOfUarts[UART_Number-1].rxBuff;
}

uint8_t* _UART_Scanf_Inst(UART_HandleTypeDef *huart){
	uint8_t uartInstance = 0;
	for(uint8_t i = 0;i<countOfUarts;i++){
		if(huart==listOfUarts[i].huartx){
			uartInstance = i+1;
			break;
		}
	}
	if(uartInstance == 0){
		return NULL;
	}
	return listOfUarts[uartInstance-1].rxBuff;
}

#endif
