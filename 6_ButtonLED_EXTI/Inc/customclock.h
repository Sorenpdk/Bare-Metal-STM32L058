/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMCLOCK_H
#define CUSTOMCLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "customdelay.h"
#include "system_stm32l0xx.h" // For SystemCoreClockUpdate

/* Private define ------------------------------------------------------------*/
#define HSI_TIMEOUT_VALUE              ((uint32_t)100)  /* 100 ms */
#define REGULATOR_TIMEOUT_VALUE        ((uint32_t)500)  /* 500 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE      ((uint32_t)5000) /* 5 s    */


extern void customClock_init(void);  
/*  ****** IMPORTANT to call SystemCoreClockUpdate() after configuring the SYS_CLK ******
  * SystemCoreClockUpdate();
  * SysTick_Config(SystemCoreClock/1000); */


#endif // CUSTOMCLOCK_H