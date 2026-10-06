/*
 * CAN_RMMotors.c
 *
 *  Created on: Sep 16, 2025
 *      Author: Genysix
 */

#include "main.h"
#ifdef INCLUDE_CAN_RMMOTORS
#include "CAN_RMMotors.h"
#include "PID.h"
#include "stdlib.h"
#include "CAN_Core.h"
#include "stdarg.h"
#include "vararg.h"

BUS_t *listOfBuses;
uint8_t activeBuses = 0;

uint8_t *busLookup;
uint8_t busLookupSize = 0;

uint8_t currentGroupSend = 0;
uint8_t updateCounter = 0;

CAN_HandleTypeDef hcan1 __attribute__((weak));
CAN_HandleTypeDef hcan2 __attribute__((weak));

CAN_LibraryInfo CAN_RMMotors(){
	CAN_LibraryInfo CAN_RMMotorsInfo;
	CAN_RMMotorsInfo.rx_passThrough_Fn = &CAN_RM_Recieve;
	return CAN_RMMotorsInfo;
}

void _CAN_RM_Init_Buses(int n_arg,...){
		va_list buses;
		va_start(buses,n_arg);

		for(int i = 0; i<n_arg;i++){
			int testArg = va_arg(buses,int);
			if(testArg==1){
					extern CAN_HandleTypeDef hcan1;
					CAN_RM_Init(&hcan1,BUS1);
			}
			if(testArg==2){
					extern CAN_HandleTypeDef hcan2;
					CAN_RM_Init(&hcan2,BUS2);

			}
		}
		va_end(buses);

		osDelay(200);
		extern TIM_HandleTypeDef htim13;
		PID_Init(&htim13);

}


/*
 * PARAMS:     CAN_HandleTypeDef  -  Pointer to can instance wanting to be initialized
 * 		   OR  CAN_HandleTypeDef, uint8_t  -  Pointer to can instance to be initialized, desired bus number to be linked to it
 *
 * RETURNS: uint8_t busNumber linked to the initialized instance. Use this number to access the
 *
 *
 */
uint8_t _CAN_RM_Init(int n_arg, ...){

	//limit number of buses to restrict memory usage in case the function is accidentally called in a loop. Prevents leakage
	if(activeBuses>=8){
		return 224;
	}

	//Go through the inputed arguments
	va_list args;
	va_start(args,n_arg);

	CAN_HandleTypeDef* hcanx;
	uint8_t busNumber = 0;

	//if there are arguments present, load the first parameter into hcanx
	if(n_arg>0){
		hcanx = va_arg(args,CAN_HandleTypeDef*);
	}

	//if the desired bus number is defined: set the bus number to this
	//else: find an empty spot in the lookup table and set that as current bus munber.
	//If the table is full, set the bus number to be the next value outside the lookup table
	if(n_arg>1){
		busNumber = (uint8_t)va_arg(args,int);
		if(busNumber<1||busNumber>8){
			return 224;
		}
	}else{
		for(uint8_t i = 0;i<busLookupSize;i++){
			if(busLookup[i]==0){
				busNumber = i + 1;
				break;
			}
		}
		if(busNumber == 0){
			busNumber = busLookupSize + 1;
		}
	}

	va_end(args);


	if(!activeBuses){

		//create an array of bus objects
		listOfBuses = (BUS_t*) malloc(sizeof(BUS_t));

		//start the lookup table
		busLookup = (uint8_t*) malloc(busNumber*sizeof(uint8_t));
		busLookupSize = busNumber;

		for(uint8_t i = 0;i<busNumber-1;i++){
			busLookup[i] = 0;
		}

	}else{

		//resize the array of bus objects to include a new one
		BUS_t *temp = realloc(listOfBuses,(activeBuses+1)*sizeof(BUS_t));
		if(temp){
			listOfBuses = temp;
		}else{
			free(temp);
			return 220;
		}

		//if the bus number to be added to the lookup table is outside of the scope increase the table scope
		if(busNumber>busLookupSize){
			uint8_t *temp2 = realloc(busLookup,(busNumber)*sizeof(uint8_t));

			if(temp2){
				busLookup = temp2;
			}else{
				free(temp2);
				return 220;
			}
			for(uint8_t i = busLookupSize;i<busNumber;i++){
				busLookup[i] = 0;
			}
			busLookupSize = busNumber;
		}

	}
	//each 1-indexed element of the look up table contains the 1-idexed corrolated listOfBuses position
	activeBuses++;
	busLookup[busNumber-1] = activeBuses;


	//initialize all values to 0
	(listOfBuses+activeBuses-1)->busPointer = hcanx;
	(listOfBuses+activeBuses-1)->busStatus = 0;
	for(uint8_t i = 0; i<8; i++){
		(listOfBuses+activeBuses-1)->listOfC610[i].actualCurrent = 0;
		(listOfBuses+activeBuses-1)->listOfC610[i].activeStatus = 0;
		(listOfBuses+activeBuses-1)->listOfC610[i].currentAngle = 0;
		(listOfBuses+activeBuses-1)->listOfC610[i].currentSpeed = 0;
		(listOfBuses+activeBuses-1)->listOfC610[i].posPIDid = 0;
		(listOfBuses+activeBuses-1)->listOfC610[i].rpmPIDid = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].actualCurrent = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].activeStatus = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].currentAngle = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].currentSpeed = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].posPIDid = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].rpmPIDid = 0;
		(listOfBuses+activeBuses-1)->listOfC620[i].actualTemp = 0;
	}
	for(uint8_t i = 0; i<7; i++){
		(listOfBuses+activeBuses-1)->listOf6020[i].actualCurrent = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].activeStatus = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].currentAngle = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].currentSpeed = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].posPIDid = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].rpmPIDid = 0;
		(listOfBuses+activeBuses-1)->listOf6020[i].actualTemp = 0;
	}
	for(uint8_t i = 0; i<4; i++){
		(listOfBuses+activeBuses-1)->txBuff[0][i] = 0;
		(listOfBuses+activeBuses-1)->txBuff[1][i] = 0;
		(listOfBuses+activeBuses-1)->txBuff[2][i] = 0;
	}
	return busNumber;
}


void CAN_RM_DisableBusSending(uint8_t busNumber){
	(listOfBuses+busLookup[busNumber-1]-1)->busStatus = 1;
}



void setC610Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>8){
		return;
	}

	if(targetPos<0){
		targetPos=0;
	}
	else if(targetPos>8191){
		targetPos=8191;
	}

	//if a c620 with ID conflict is active, do not execute
	if((listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus){
		return;
	}
	//if a 6020 with ID conflict is active, do not execute
	if(motorID>4&&(listOfBuses+busOffset-1)->listOf6020[motorID-5].activeStatus){
		return;
	}

	//if the controller does not have a pos PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Pos;
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_RPM):{
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_RPM;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}

	//find closest displacement to target accomodating encoder reset
		int16_t currentError = targetPos-get6020Pos(busNumber, motorID);
		if(currentError>4096){
			currentError = -(8191 - currentError);
		}else if(currentError<-4095){
			currentError = -(-8191 - currentError);
		}



	int16_t drivingCurrent = 0.30518*(float)PID_Calculate(targetPos,targetPos-currentError,PIDTune,(listOfBuses+busOffset-1)->listOfC610[motorID-1].posPIDid);

	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,0,motorID-1, drivingCurrent);
	}else{
		CAN_RM_SetCurrent(busNumber,1,motorID-5, drivingCurrent);
	}
}



void setC610RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>8){
		return;
	}

	//if a c620 with ID conflict is active, do not execute
	if((listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus){
		return;
	}

	//if a 6020 with ID conflict is active, do not execute
	if(motorID>4&&(listOfBuses+busOffset-1)->listOf6020[motorID-5].activeStatus){
		return;
	}

	//if the controller does not have a rpm PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_RPM;
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_Pos):{
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus = CGM_PID_Pos;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}


	int16_t drivingCurrent = 0.30518*(float)PID_Calculate(targetRPM,getC610RPM(busNumber,motorID),PIDTune,(listOfBuses+busOffset-1)->listOfC610[motorID-1].rpmPIDid);

	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,0,motorID-1, drivingCurrent);
	}else{
		CAN_RM_SetCurrent(busNumber,1,motorID-5, drivingCurrent);
	}
}


void setC620Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>8){
		return;
	}

	//if a c610 with ID conflict is active, do not execute
	if((listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus){
		return;
	}

	//if a 6020 with ID conflict is active, do not execute
	if(motorID>4&&(listOfBuses+busOffset-1)->listOf6020[motorID-5].activeStatus){
		return;
	}

	//if the controller does not have a pos PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Pos;
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_INITIALIZATION_ERROR){
				(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_RPM):{
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid==PID_INITIALIZATION_ERROR){
				(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_RPM;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}

	//find closest displacement to target accomodating encoder reset
	int16_t currentError = targetPos-get6020Pos(busNumber, motorID);
	if(currentError>4096){
		currentError = -(8191 - currentError);
	}else if(currentError<-4095){
		currentError = -(-8191 - currentError);
	}


	int16_t drivingCurrent = 0.5*(float)PID_Calculate(targetPos,targetPos-currentError,PIDTune,(listOfBuses+busOffset-1)->listOfC620[motorID-1].posPIDid);

	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,0,motorID-1, drivingCurrent);
	}else{
		CAN_RM_SetCurrent(busNumber,1,motorID-5, drivingCurrent);
	}
}


void setC620RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>8){
		return;
	}

	//if a c610 with ID conflict is active, do not execute
	if((listOfBuses+busOffset-1)->listOfC610[motorID-1].activeStatus){
		return;
	}

	//if a 6020 with ID conflict is active, do not execute
	if(motorID>4&&(listOfBuses+busOffset-1)->listOf6020[motorID-5].activeStatus){
		return;
	}

	//if the controller does not have a rpm PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_RPM;
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_Pos):{
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_INITIALIZATION_ERROR||(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid==PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOfC620[motorID-1].activeStatus = CGM_PID_Pos;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}
	

	int16_t drivingCurrent = 0.5*(float)PID_Calculate(targetRPM,getC620RPM(busNumber,motorID),PIDTune,(listOfBuses+busOffset-1)->listOfC620[motorID-1].rpmPIDid);

	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,0,motorID-1, drivingCurrent);
	}else{
		CAN_RM_SetCurrent(busNumber,1,motorID-5, drivingCurrent);
	}
}


void set6020Pos(uint8_t busNumber, uint8_t motorID, int16_t targetPos, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>7){
		return;
	}

	if(targetPos<0){
		targetPos = 0;
	}
	else if(targetPos>8191){
		targetPos = 8191;
	}

	//if a c610 or c620 with ID conflict is active, do not execute
	if((motorID<5&&(listOfBuses+busOffset-1)->listOfC610[motorID+3].activeStatus)||(motorID<5&&(listOfBuses+busOffset-1)->listOfC620[motorID+3].activeStatus)){
		return;
	}

	//if the controller does not have a pos PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Pos;
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_INITIALIZATION_ERROR){
				(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_RPM):{
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid = PID_New();

			if((listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_STORAGE_FULL_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid==PID_INITIALIZATION_ERROR){
				(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_RPM;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}

	//find closest displacement to target accomodating encoder reset
	int16_t currentError = targetPos-get6020Pos(busNumber, motorID);
	if(currentError>4096){
		currentError = -(8191 - currentError);
	}else if(currentError<-4095){
		currentError = -(-8191 - currentError);
	}


	int16_t drivingVoltage = 0.7629*(float)PID_Calculate(targetPos,targetPos-currentError,PIDTune,(listOfBuses+busOffset-1)->listOf6020[motorID-1].posPIDid);


	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,1,motorID-1, drivingVoltage);
	}else{
		CAN_RM_SetCurrent(busNumber,2,motorID-5, drivingVoltage);
	}

}


void set6020RPM(uint8_t busNumber, uint8_t motorID, int16_t targetRPM, float PIDTune[3]){

	uint8_t busOffset = busLookup[busNumber-1];



	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(motorID<1||motorID>7){
		return;
	}

	//if a c610 or c620 with ID conflict is active, do not execute
	if((motorID<5&&(listOfBuses+busOffset-1)->listOfC610[motorID+3].activeStatus)||(motorID<5&&(listOfBuses+busOffset-1)->listOfC620[motorID+3].activeStatus)){
		return;
	}

	//if the controller does not have a rpm PID attached to it, make one and update status byte accordingly
	switch((listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus){
		case(CGM_PID_Null):{
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_RPM;
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid = PID_New();

			//check if PID creation failed, deactivate the motors pid and return
			if((listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_INITIALIZATION_ERROR ||(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Null;
				return;
			}
			break;
		}
		case(CGM_PID_Pos):{
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Dual;
			(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid = PID_New();

			//check if PID creation failed, deactivate the motors pid and return
			if((listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_INITIALIZATION_ERROR ||(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_REALLOCATION_ERROR||(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid == PID_STORAGE_FULL_ERROR){
				(listOfBuses+busOffset-1)->listOf6020[motorID-1].activeStatus = CGM_PID_Pos;
				return;
			}
			break;
		}
		default:{
			break;
		}
	}

	int16_t drivingVoltage = 0.7629*(float)PID_Calculate(targetRPM,get6020RPM(busNumber,motorID),PIDTune,(listOfBuses+busOffset-1)->listOf6020[motorID-1].rpmPIDid);

	if(motorID<5){
		CAN_RM_SetCurrent(busNumber,1,motorID-1, drivingVoltage);
	}else{
		CAN_RM_SetCurrent(busNumber,2,motorID-5, drivingVoltage);
	}
}




void CAN_RM_SetCurrent(uint8_t busNumber, uint8_t groupNumber, uint8_t packagePos, int16_t current){

	uint8_t busOffset = busLookup[busNumber-1];

	if(!activeBuses){
		return;
	}

	if(busOffset<1||busOffset>activeBuses){
		return;
	}

	if(groupNumber<0||groupNumber>2){
		return;
	}

	if(packagePos<0||packagePos>4){
		return;
	}

	//flip higher order and lower order bits
	int16_t txBuff = (((current & 0xff00)>>8)|((current & 0x00ff)<<8));

	//write the current send value to
	(listOfBuses+busOffset-1)->txBuff[groupNumber][packagePos] = txBuff;
}


void CAN_RM_Update(){
	for(uint8_t i = 0; i<activeBuses; i++){
		if((listOfBuses+i)->busStatus){
			continue;
		}
		switch(currentGroupSend){
			case(0):{
				CAN_Transmit((listOfBuses+i)->busPointer,0x200,(listOfBuses+i)->txBuff[0]);
				break;
			}
			case(1):{
				CAN_Transmit((listOfBuses+i)->busPointer,0x1ff,(listOfBuses+i)->txBuff[1]);
				break;
			}
			case(2):{
				CAN_Transmit((listOfBuses+i)->busPointer,0x2ff,(listOfBuses+i)->txBuff[2]);
				break;
			}
			default:{
				break;
			}
		}

	}
	if(currentGroupSend<2){
		currentGroupSend++;
	}else{
		currentGroupSend = 0;
	}

}


void CAN_RM_Recieve(CAN_HandleTypeDef *hcan, uint32_t StdId,uint32_t ExtId, uint8_t rx_data[8]){


	if(!activeBuses){
		return;
	}

	uint8_t busNumber = 0;
	for(uint8_t i=0; i<activeBuses; i++){
		if(hcan == (listOfBuses+i)->busPointer){
			busNumber = i+1;
			break;
		}
	}


	switch(StdId){
		case(CAN_C6x0_1_ID):
		case(CAN_C6x0_2_ID):
		case(CAN_C6x0_3_ID):
		case(CAN_C6x0_4_ID):
		{
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].actualTemp = rx_data[6];
			break;
		}
		case(CAN_CGMOverlap_1_ID):
		case(CAN_CGMOverlap_2_ID):
		case(CAN_CGMOverlap_3_ID):
		case(CAN_CGMOverlap_4_ID):
		{
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOfC610[StdId-CAN_C6x0_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOfC620[StdId-CAN_C6x0_ID_OFFSET].actualTemp = rx_data[6];
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].actualTemp = rx_data[6];
			break;
		}
		case(CAN_GM6020_1_ID):
		case(CAN_GM6020_2_ID):
		case(CAN_GM6020_3_ID):
		{
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].currentAngle = (rx_data[1]|(rx_data[0]<<8));
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].currentSpeed = (rx_data[3]|(rx_data[2]<<8));
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].actualCurrent = rx_data[5]|(rx_data[4]<<8);
			(listOfBuses+busNumber-1)->listOf6020[StdId-CAN_GM6020_ID_OFFSET].actualTemp = rx_data[6];
			break;
		}
		default:{
			break;
		}
	}
}


int16_t getC610Pos(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<9){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOfC610[id-1].currentAngle;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t getC610RPM(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<9){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOfC610[id-1].currentSpeed;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t getC620Pos(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<9){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOfC620[id-1].currentAngle;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t getC620RPM(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<9){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOfC620[id-1].currentSpeed;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t get6020Pos(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<8){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOf6020[id-1].currentAngle;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t get6020RPM(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<8){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOf6020[id-1].currentSpeed;
	}else{
		return 0;
	}
	}else{return 0;}
}

int16_t get6020Temp(uint8_t busNumber, uint8_t id){
	if(activeBuses){
	if(id>0&&id<8){
		return (listOfBuses+busLookup[busNumber-1]-1)->listOf6020[id-1].actualTemp;
	}else{
		return 0;
	}
	}else{return 0;}
}
#endif


