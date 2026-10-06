/*
 * PID.h
 *
 *  Created on: Sep 15, 2025
 *      Author: Gen6
 */

#include "main.h"
#ifdef INCLUDE_PID
#ifndef INC_PID_H_
#define INC_PID_H_


typedef struct {
    uint16_t lastSampleTime;
    int16_t lastErrorValue;
    float errorSum;
    uint8_t activePID;
} PID_data_t;

typedef enum {

	PID_REALLOCATION_ERROR = 220,
	PID_INITIALIZATION_ERROR = 222,
	PID_STORAGE_FULL_ERROR = 224,

}PID_ERROR_CODE;

int16_t PID_Calculate(int16_t target, int16_t current, float PIDTune[3], uint8_t PIDid);

void PID_Update();

uint8_t PID_New();

void PID_Init(TIM_HandleTypeDef *htimxinit);

uint16_t PID_GetDT(uint16_t currentTime, uint16_t lastSample);

#endif /* INC_PID_H_ */
#endif
