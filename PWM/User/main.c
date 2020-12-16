/**
  ******************************************************************************
  * @file    main.c
  * @author  Søren Pørksen
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *            Using Timer 2 Channel 1 to generate a PWM signal and outputting
  *            to Port A pin 0.
  *            
  *          
  ******************************************************************************
 */ 


/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
void ConfigureGPIO(void);
void PWM_Init();
/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main (void)
{
  
  ConfigureGPIO();
  PWM_Init();
  
  while(1)
  {
    
  }
}

/**
  * Brief   This function enables the peripheral clock on GPIO port A
  *         
  *         configures GPIO PA0 to output TIM2_CH1 (PWM)
  * Param   None
  * Retval  None
  */
__INLINE void ConfigureGPIO(void)
{  
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin 5 */
  /* (3) Select Alternate function mode (10) on GPIOA pin 0 */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN; /* (1) */  
  
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE0)) 
               | (GPIO_MODER_MODE0_1); /* (2) */
  
  GPIOA->AFR[0] |= 0x2; /* (3) */  

}

/**
  * Brief   This function enables the peripheral clock on APB1
  *         and configures Timer 2 Channel 1 to output a PWM signal
  *         
  *         configures GPIO PA0 to output TIM2_CH1 (PWM ~100Hz ~50% DC)
  * Param   None
  * Retval  None
  */
__INLINE void PWM_Init()
{
  /******************** INFO ***********************/
  /* TIM2_CH1 mapped to PA0 as alternate function */
  /* By default running with MSI 2.097 MHz SYS_CLK */ 
  /* By default APB_CLK is SYS_CLK prescaled  1 so ~2MHz*/  
  /* Freq(Hz) = APB_CLK / (PSC +1) * (ARR +1) */
  /* DutyCycle(%) = (CRRx*100) / ARR */
  /* Period(s) = (PSC + 1) * (ARR +1) / APB_CLK */
  /* Period(ms) = 1000 * (PSC + 1) * (ARR +1) / APB_CLK */
  /******************************************************/
 
  /* (0) Enable the peripheral clock of Timer x */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* (0) */
  
  /* (1) Set prescaler, so APB_CLK/512*40 = 100 Hz 
   SYS_CLK_PSC / (PSC[15:0] + 1) = 2.097 MHz / 512 * 40 = 102 Hz  */
  /* (2) Set ARR = 39, as timer clock is 2.097 MHz the period is 9.76 ms */
  /* (3) Set CCRx accordingly to achieved specific duty cycle.
  /* (4) Select PWM mode 1 on OC1 (OC1M = 110),
  enable preload register on OC1 (OC1PE = 1) */
  /* (5) Select active high polarity on OC1 (CC1P = 0, reset value),
  enable the output on OC1 (CC1E = 1) */
  /* (6) Enable output (MOE = 1)- optional*/
  /* (7) Enable counter (CEN = 1)
  select edge aligned mode (CMS = 00, reset value)
  select direction as upcounter (DIR = 0, reset value) */
  /* (8) Force update generation (UG = 1) */ 
  
   
  TIM2->PSC = 511; /* (1) */
  TIM2->ARR = 39; /* (2) */
  TIM2->CCR1 = 19; /* (3) */
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1
 | TIM_CCMR1_OC1PE; /* (4) */
  TIM2->CCER |= TIM_CCER_CC1E; /* (5) */
  TIM2->CR1 |= TIM_CR1_CEN; /* (6) */
  TIM2->EGR |= TIM_EGR_UG; /* (7) */
  
}
