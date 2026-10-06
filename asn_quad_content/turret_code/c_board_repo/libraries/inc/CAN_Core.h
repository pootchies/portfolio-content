/*
 * CAN_Core.h
 *
 *  Created on: Sep 13, 2025
 *      Author: Genysix
 *      Version: BETA 0.3
 */
#include "main.h"
#ifdef INCLUDE_CAN_CORE
#ifndef INC_CAN_CORE_H_
#define INC_CAN_CORE_H_
#include "vararg.h"

typedef enum {

	BUS1 = 1,
	BUS2 = 2,

}CAN_BUS;

typedef struct{
	void (*rx_passThrough_Fn)(CAN_HandleTypeDef*, uint32_t, uint32_t, uint8_t*);
}CAN_LibraryInfo;


#define CAN_LinkLib(...) _CAN_LinkLib(PP_NARG(__VA_ARGS__), __VA_ARGS__)

void _CAN_LinkLib(int n_arg, ...);

void CAN_Transmit(CAN_HandleTypeDef *hcanx, uint32_t stdId, uint8_t data[8]);

void CAN_Transmit_Ex(CAN_HandleTypeDef *hcanx, uint32_t extId, uint8_t data[8]);

void CAN_Init(CAN_HandleTypeDef *hcanx, uint32_t filterBank, uint32_t slaveStartBank);

#define CAN_Init_Buses(...) _CAN_Init_Buses(PP_NARG(__VA_ARGS__), __VA_ARGS__)

void _CAN_Init_Buses(int n_arg,...);


#endif /* INC_CAN_CORE_H_ */
#endif
