/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMUART_H
#define CUSTOMUART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "stdlib.h"


extern void customUSART1_Init(void);
extern void UART_Send_Char(uint8_t c);
extern void UART_Send_String(char* msg);

extern uint8_t UART_Receive_Char();


#endif // CUSTOMUART_H