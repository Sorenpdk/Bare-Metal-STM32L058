/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMUART_H
#define CUSTOMUART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"


/* Private define ------------------------------------------------------------*/
extern void customUART_init(void);
extern void UART_Send_Char(unsigned char c);
extern void UART_Send_String(const char* msg);

#endif // CUSTOMUART_H_H