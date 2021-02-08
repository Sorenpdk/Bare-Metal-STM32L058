/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *             
  *            
  *          
  ******************************************************************************
 */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "customgpio.h"
#include "customdelay.h"
#include "customclock.h"

void Counter_Init();

/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
static uint8_t test;

int main (void)
{
  customClock_init();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  customGPIO_init();
  
  Counter_Init();
  
  while(1)
  {
    toggleLED2();  
    customDelay(500);
  }
}

/**
  * Brief   This function enables the peripheral clock on APB1
  *         and configures Timer6 as an up counter.
  *         Each overflow causes an interrupt which increments the test variable.
  *         
  * Param   None
  * Retval  None
  */
__INLINE void Counter_Init()
{
  /******************** INFO ***********************/
  /* Using basic timer Timer 6 */
  /* Custom clock is PLL24MHz(HSI16) as SYS_CLK */ 
  /* By default APB2_CLK is SYS_CLK prescaled  2 so 12MHz*/  
  /* Period(s) = (PSC + 1) * (ARR +1) / APB_CLK */
  /* Period(s) = (373 + 1) * (32084 + 1) / 12*10^6 = 0.9999825 s  */
  /******************************************************/
 
  /* (0) Enable the peripheral clock of Timer x */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM6EN;/* (0) */
  
  /* (1) Set prescaler, so APB_CLK/374 =  */
  /* (2) Set ARR = 32085 to overflow every 1 second  */
  /* (3) Enable interrupt update */
  /* (4) Enable the counter */
  /* (5) Enable the NVIC interrupt */
  /* (6) Select interrupt priority */
  
  TIM6->PSC = 374; /* (1) */
  TIM6->ARR = 32085; /* (2) */
  TIM6->DIER = 1UL << 0;  /* (3) */
  TIM6->CR1 |= 0x1; /* (4) */
  NVIC_EnableIRQ(TIM6_DAC_IRQn); /* (5) */
  NVIC_SetPriority(TIM6_DAC_IRQn,0); /* (6) */

}

void TIM6_DAC_IRQHandler()
{
  TIM6->SR = 0; // Clear Update interrupt flag
  test++;
} 



/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

