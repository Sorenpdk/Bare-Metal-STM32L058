/**
  ******************************************************************************
  * @file    customclock.h
  * @author  Søren Pørksen      
  * @brief   CMSIS Cortex-M0+ Device Custom Clock settings Header File. 
  *          This file contains the function protoype of custom_clk_config()
  *          
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOMCLOCK_H
#define CUSTOMCLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"


extern void Custom_clk_config(void);


#endif // CUSTOMCLOCK_H