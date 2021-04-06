/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMDELAY_H
#define CUSTOMDELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "stm32l0xx_it.h" // For SysTick_Handler 
#include "system_stm32l0xx.h" // For SystemCoreClockUpdate
  
 /* Private define ------------------------------------------------------------*/
#define SEC_DELAY ((uint32_t)1000)

extern void customDelay_init(void);
extern void customDelay(uint32_t ms);


#endif // CUSTOMDELAY_H