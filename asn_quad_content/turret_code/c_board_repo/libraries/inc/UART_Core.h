/*
 * UART_Core.h
 *
 *  Created on: Apr 7, 2026
 *      Author: Gen6
 */

#include "main.h"
#ifdef INCLUDE_UART_CORE
#ifndef INC_UART_CORE_H_
#define INC_UART_CORE_H_
#include "vararg.h"

typedef enum {
	UART1 = 1,
	UART2 = 2,
}UART_INSTANCE;

typedef struct{
	UART_HandleTypeDef* huartx;
	uint8_t rxBuff[256];
}UART_Instance_t;

typedef struct{
	void (*rx_passThrough_Fn)(UART_HandleTypeDef *, uint8_t[256]);
}UART_LibraryInfo;



#define UART_LinkLib(...) _UART_LinkLib(PP_NARG(__VA_ARGS__), __VA_ARGS__)

void _UART_LinkLib(int n_arg, ...);



void UART_Init();

uint8_t UART_Ex_Init(UART_HandleTypeDef *huart);


#ifdef __CDT_PARSER__
    #define UART_Printf(UART_INSTANCE, STRING, ...) (void)0
#else
#define UART_Printf(...) _Generic((GET_FIRST_(__VA_ARGS__)), \
	    int:    _UART_Printf_Num, \
		UART_HandleTypeDef *:  _UART_Printf_Inst \
	)(__VA_ARGS__)
#endif


void _UART_Printf_Num(uint8_t UART_Number,const char *fmt,...);

void _UART_Printf_Inst(UART_HandleTypeDef *huart,const char *fmt,...);



#ifdef __CDT_PARSER__
    #define UART_Scanf(UART_INSTANCE) (void)0
#else
    #define UART_Scanf(...) _Generic((GET_FIRST_(__VA_ARGS__)), \
        int:    _UART_Scanf_Num, \
        UART_HandleTypeDef *:  _UART_Scanf_Inst \
    )(GET_FIRST_(__VA_ARGS__))
#endif

uint8_t* _UART_Scanf_Num(uint8_t UART_Number);

uint8_t* _UART_Scanf_Inst(UART_HandleTypeDef *huart);

#endif /* INC_UART_CORE_H_ */
#endif
