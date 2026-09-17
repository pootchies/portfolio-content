/*
 * CAN_RMMotors.h
 *
 *  Created on: Sep 16, 2025
 *      Author: Gen6
 */

#include "main.h"
#ifdef INCLUDE_CAN_RMMOTORS
#ifndef INC_CAN_RMMOTORS_H_
#define INC_CAN_RMMOTORS_H_
#include "vararg.h"




typedef enum {
	//group identifiers for group 2 of M3508 are the same as group 1 of GM6020 (voltage ctrl)
    CAN_C6x0_ID = 0x200,
	CAN_CGMOverlap_ID = 0x1FF,
	CAN_GM6020_ID = 0x2FF,

	CAN_GM6020_Current_1_ID = 0x1FE,
	CAN_GM6020_Current_2_ID = 0x2FE,


	CAN_C6x0_1_ID = 0x201,
	CAN_C6x0_2_ID = 0x202,
	CAN_C6x0_3_ID = 0x203,
	CAN_C6x0_4_ID = 0x204,

	CAN_CGMOverlap_1_ID = 0x205,
	CAN_CGMOverlap_2_ID = 0x206,
	CAN_CGMOverlap_3_ID = 0x207,
	CAN_CGMOverlap_4_ID = 0x208,

	CAN_GM6020_1_ID = 0x209,
	CAN_GM6020_2_ID = 0x20A,
	CAN_GM6020_3_ID = 0x20B,


	CAN_C6x0_ID_OFFSET = 0x201,
	CAN_GM6020_ID_OFFSET = 0x205,
}CAN_ID;

typedef enum {
    CGM_PID_Null =0,
	CGM_PID_Pos = 1,
	CGM_PID_RPM = 2,
	CGM_PID_Dual = 3,
}CGM_PID_STATUS;


typedef struct {
    int16_t currentAngle;
    int16_t currentSpeed;
    int16_t actualCurrent;
	uint8_t activeStatus;
	uint8_t posPIDid;
	uint8_t rpmPIDid;
} C610_t;

typedef struct {
	int16_t currentAngle;
	int16_t currentSpeed;
	int16_t actualCurrent;
	int8_t actualTemp;
	uint8_t activeStatus;
	uint8_t posPIDid;
	uint8_t rpmPIDid;
} C620_t;

typedef struct {
	int16_t currentAngle;
	int16_t currentSpeed;
	int16_t actualCurrent;
	int8_t actualTemp;
	uint8_t activeStatus;
	uint8_t posPIDid;
	uint8_t rpmPIDid;
} GM6020_t;


typedef struct {
	C610_t listOfC610[8];
	C620_t listOfC620[8];
	GM6020_t listOf6020[7];
	int16_t txBuff[3][4];
	CAN_HandleTypeDef* busPointer;
	uint8_t busStatus;
} BUS_t;


void setC610Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]);

void setC610RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]);

void setC620Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]);

void setC620RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]);

void set6020Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]);

void set6020RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]);

void CAN_RM_DisableBusSending(uint8_t busNumber);

void CAN_RM_SetCurrent(uint8_t busNumber, uint8_t groupNumber, uint8_t packagePos, int16_t current);

#define CAN_RM_Init(...) _CAN_RM_Init(PP_NARG(__VA_ARGS__), __VA_ARGS__)

uint8_t _CAN_RM_Init(int n_arg, ...);

void CAN_RM_Update();

void CAN_RM_Recieve(CAN_HandleTypeDef *hcan, uint32_t StdId, uint32_t ExtId, uint8_t rx_data[8]);

CAN_LibraryInfo CAN_RMMotors();


#define CAN_RM_Init_Buses(...) _CAN_RM_Init_Buses(PP_NARG(__VA_ARGS__), __VA_ARGS__)

void _CAN_RM_Init_Buses(int n_arg,...);

int16_t getC610Pos(uint8_t busNumber, uint8_t id);

int16_t getC610RPM(uint8_t busNumber, uint8_t id);

int16_t getC620Pos(uint8_t busNumber, uint8_t id);

int16_t getC620RPM(uint8_t busNumber, uint8_t id);

int16_t getC620Temp(uint8_t busNumber, uint8_t id);

int16_t get6020Pos(uint8_t busNumber, uint8_t id);

int16_t get6020RPM(uint8_t busNumber, uint8_t id);

int16_t get6020Temp(uint8_t busNumber, uint8_t id);

#endif /* INC_CAN_RMMOTORS_H_ */
#endif
