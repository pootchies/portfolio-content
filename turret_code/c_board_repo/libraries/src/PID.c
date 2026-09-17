/*
 * PID.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Genysix
 */

#include "main.h"
#ifdef INCLUDE_PID
#include "PID.h"
#include "stdlib.h"
#include "float.h"



TIM_HandleTypeDef *htimx;

PID_data_t *PIDArrayPnt;
uint8_t PIDArrayLen = 0;
uint8_t isInitialized = 0;


int16_t PID_Calculate(int16_t target, int16_t current, float PIDTune[3], uint8_t PIDid){
	//has the timer been started and array initialized?
	if(!isInitialized){
		return 0;
	}

	//is PIDid Valid
	if(PIDid<0||PIDid>=PIDArrayLen){
		return 0;
	}


	uint16_t currentTime = __HAL_TIM_GET_COUNTER(htimx);
	float error = target-current;
	float output = 0;
	float errorDiff = PIDTune[2]*(error-(float)(PIDArrayPnt+PIDid)->lastErrorValue)/(float)PID_GetDT(currentTime, (PIDArrayPnt+PIDid)->lastSampleTime)*1000.0;


	if((PIDArrayPnt+PIDid)->activePID){

		//add to error sum
		(PIDArrayPnt+PIDid)->errorSum += PIDTune[1]*(error*(((float)PID_GetDT(currentTime, (PIDArrayPnt+PIDid)->lastSampleTime))/1000.0));

		//cap errorSum to stop overflow
		if((PIDArrayPnt+PIDid)->errorSum>32767){
			(PIDArrayPnt+PIDid)->errorSum = 32767;
		}else if((PIDArrayPnt+PIDid)->errorSum<-32768){
			(PIDArrayPnt+PIDid)->errorSum = -32768;
		}

		if(PIDTune[1]==0.0){
			(PIDArrayPnt+PIDid)->errorSum = 0;
		}

		output = PIDTune[0]*error+(PIDArrayPnt+PIDid)->errorSum+errorDiff;

	}else{

		output = PIDTune[0]*error;
		(PIDArrayPnt+PIDid)->errorSum = 0;

	}
	//cap output for int16
	if(output>32767){
		output = 32767;
	}else if(output<-32768){
		output = -32768;
	}

	//set active
	(PIDArrayPnt+PIDid)->activePID=1;

	//update lastErrorVal
	(PIDArrayPnt+PIDid)->lastErrorValue = error;

	//update lastErrorVal
	(PIDArrayPnt+PIDid)->lastSampleTime = currentTime;

	return (int16_t)output;

}

//check to see if a PID needs to be timed out. If time since last sample is >30ms times out the
//pid so that i term doesnt blow up
void PID_Update(){
	if(isInitialized){
		for(uint8_t i = 0; i<PIDArrayLen;i++){
			if((PID_GetDT(__HAL_TIM_GET_COUNTER(htimx), (PIDArrayPnt+i)->lastSampleTime)>30000)&&((PIDArrayPnt+i)->activePID == 1)){
				(PIDArrayPnt+i)->activePID = 0;
			}

		}
	}
}

//returns new PID pid tag
//if returns 220, reallocation error occured
//if returns 222, not initialized
//if returns 224, storage is full
uint8_t PID_New(){
	if(isInitialized){
		if(PIDArrayLen<200){
			if(PIDArrayLen!=0){
				PID_data_t *temp = realloc(PIDArrayPnt,(PIDArrayLen+1)*sizeof(PID_data_t));
				if(temp)
				{
					free(PIDArrayPnt);
					PIDArrayPnt = temp;
				}else{
					free(temp);
					return PID_REALLOCATION_ERROR;
				}
			}
			PIDArrayLen++;

			//init to zeros

			(PIDArrayPnt+PIDArrayLen-1)->lastSampleTime = 0;
			(PIDArrayPnt+PIDArrayLen-1)->lastErrorValue = 0;
			(PIDArrayPnt+PIDArrayLen-1)->errorSum = 0;
			(PIDArrayPnt+PIDArrayLen-1)->activePID = 0;

			return PIDArrayLen-1;
		}else{
			return PID_STORAGE_FULL_ERROR;
		}
	}
	return PID_INITIALIZATION_ERROR;
}


void PID_Init(TIM_HandleTypeDef *htimxinit){
	if(!isInitialized){
	PIDArrayPnt = (PID_data_t*) malloc(sizeof(PID_data_t));
	htimx = htimxinit;
	HAL_TIM_Base_Start(htimx);
	isInitialized=1;
	}
}


uint16_t PID_GetDT(uint16_t currentTime, uint16_t lastSample){
	int32_t dTBuff =(int32_t)currentTime - (int32_t)lastSample;

	if(dTBuff<0){
		dTBuff += 65535;
	}else if(dTBuff==0){
		dTBuff = 65535;
	}
	return dTBuff;
}
#endif

