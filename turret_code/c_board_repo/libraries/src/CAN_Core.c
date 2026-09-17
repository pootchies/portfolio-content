/*
 * CAN_Core.c
 *
 *  Created on: Sep 13, 2025
 *      Author: Gen6
 *      Version:BETA 0.3
 */

#include "main.h"
#ifdef INCLUDE_CAN_CORE
#include "CAN_Core.h"
#include "stdarg.h"
#include "stdlib.h"
#include "setjmp.h"


CAN_LibraryInfo* CAN_Libraries;
uint8_t CAN_Libraries_Count = 0;

CAN_HandleTypeDef hcan1 __attribute__((weak));
CAN_HandleTypeDef hcan2 __attribute__((weak));


int16_t busvoltage = 0;
uint16_t relay1 = 0;
uint16_t relay2 = 0;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//    if(rx_header.StdId==0xA7){
//    	busvoltage = (((int16_t)rx_data[0])|(((int16_t)rx_data[1])<<8));
//    	UART_Printf(UART2,"BusVoltage=%d,Relay1=%d,Relay2=%d,",busvoltage,relay1,relay2);
//    }
//    if(rx_header.StdId==0xAA){
//		relay1 = (rx_data[3]&0x01)*2000;
//		relay2 = ((rx_data[3]&0x02)>>1)*2000;
//		UART_Printf(UART2,"BusVoltage=%d,Relay1=%d,Relay2=%d,",busvoltage,relay1,relay2);
//	}

    //UART_Printf(UART2,)
    //UART_Printf(UART2,"Transmission received: HEAD: %X   DATA:  %X %X %X %X %X %X %X %X\r\n", rx_header.StdId,rx_data[0],rx_data[1], rx_data[2], rx_data[3], rx_data[4], rx_data[5],rx_data[6],rx_data[7]);
    for(uint8_t i = 0; i<CAN_Libraries_Count;i++){
		CAN_Libraries[i].rx_passThrough_Fn(hcan,rx_header.StdId,rx_header.ExtId,rx_data);
	}

}




void _CAN_LinkLib(int n_arg, ...){
	va_list libraries;
	va_start(libraries,n_arg);
	if(CAN_Libraries_Count!=0){
		free(CAN_Libraries);
	}
	CAN_Libraries = calloc(n_arg,sizeof(CAN_LibraryInfo));
	CAN_Libraries_Count = n_arg;
	if(CAN_Libraries==NULL){
		CAN_Libraries_Count = 0;
	}
	for(int i = 0; i<n_arg;i++){
		CAN_Libraries[i] = va_arg(libraries,CAN_LibraryInfo);
	}
	va_end(libraries);

}

void CAN_Transmit(CAN_HandleTypeDef *hcanx, uint32_t stdId, uint8_t data[8]) {
    CAN_TxHeaderTypeDef txHeader;
	uint32_t send_mail_box;
	txHeader.StdId = stdId;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = 0x08;
	HAL_CAN_AddTxMessage(hcanx, &txHeader, data, &send_mail_box);
}

void CAN_Transmit_Ex(CAN_HandleTypeDef *hcanx, uint32_t extId, uint8_t data[8]) {
    CAN_TxHeaderTypeDef txHeader;
	uint32_t send_mail_box;
	txHeader.ExtId = extId;
	txHeader.IDE = CAN_ID_EXT;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = 0x08;
	HAL_CAN_AddTxMessage(hcanx, &txHeader, data, &send_mail_box);

}



void CAN_Init(CAN_HandleTypeDef *hcanx, uint32_t filterBank,uint32_t slaveStartBank) {
    CAN_FilterTypeDef CAN_filter;

    CAN_filter.FilterActivation = ENABLE;

    CAN_filter.FilterIdHigh = 0x0000;
    CAN_filter.FilterIdLow = 0x0000;
    CAN_filter.FilterMaskIdHigh = 0x0000;
    CAN_filter.FilterMaskIdLow = 0x0000;
    CAN_filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_filter.FilterScale = CAN_FILTERSCALE_32BIT;

    CAN_filter.FilterBank = filterBank;
    //CAN_filter.SlaveStartFilterBank = slaveStartBank;

    HAL_CAN_ConfigFilter(hcanx, &CAN_filter);
    HAL_CAN_Start(hcanx);
    HAL_CAN_ActivateNotification(hcanx, CAN_IT_RX_FIFO0_MSG_PENDING);
}


void _CAN_Init_Buses(int n_arg,...){
	va_list buses;
		va_start(buses,n_arg);

		for(int i = 0; i<n_arg;i++){
			int testArg = va_arg(buses,int);
			if(testArg==1){
					extern CAN_HandleTypeDef hcan1;
					CAN_Init(&hcan1,0,0);
			}
			if(testArg==2){
					extern CAN_HandleTypeDef hcan2;
					CAN_Init(&hcan2,14,13);
			}
		}
		va_end(buses);
}

#endif
