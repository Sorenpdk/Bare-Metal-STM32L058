/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMGPIO_H
#define CUSTOMGPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"



extern void customGPIO_init(void);
extern void toggleLED2(void);


#endif // CUSTOMGPIO_H