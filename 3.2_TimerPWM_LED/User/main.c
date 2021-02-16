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
#include "customgpio.h"
#include "customdelay.h"
#include "customclock.h"

void PWM_Init();
void fadeupdown(uint8_t var);
void fastblink(uint32_t period);

volatile uint8_t fadeupflag = 1;
/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main (void)
{
  customClock_init();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  customGPIO_init();
  PWM_Init();
  
  
  
  while(1)
  {
    /** BLOCKING CALLS **/ 
    /*
   fastblink(120);
   customDelay(50);
   fastblink(25);
   customDelay(50);
   fastblink(120);
   customDelay(1000);
   */
   
   fadeupdown(3);
   
  }
  
}

void fastblink(uint32_t period)
{
  
  TIM2->CCR1 = 230;
  customDelay(25);
  TIM2->CCR1 = 0;
  customDelay(period);
   
}


void fadeupdown(uint8_t var)
{
   switch(fadeupflag)
    {
    case 0:
      TIM2->CCR1 -= var;
      if(TIM2->CCR1 < 1)
      {
        fadeupflag = 1;
      }
      break;
    case 1:
      TIM2->CCR1 += var;    
      if(TIM2->CCR1 > 230)
      {
        fadeupflag = 0;
      }
      break;   
    }
   customDelay(15);
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
  /* Custom clock is PLL24MHz(HSI16) as SYS_CLK */ 
  /* By default APB2_CLK is SYS_CLK prescaled  2 so 12MHz*/  
  /* Freq(Hz) = APB_CLK / (PSC +1) * (ARR +1) */
  /* DutyCycle(%) = (CRRx*100) / ARR */
  /* Period(s) = (PSC + 1) * (ARR +1) / APB_CLK */
  /* Period(ms) = 1000 * (PSC + 1) * (ARR +1) / APB_CLK */
  /******************************************************/
 
  /* (0) Enable the peripheral clock of Timer x */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* (0) */
  
  /* (1) Set prescaler, so APB_CLK/512*234 = 100 Hz */
  /* (2) Set ARR = 233, as timer clock is 12 MHz the period is 9.98 ms */
  /* (3) Set CCRx accordingly to achieved specific duty cycle. */
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
  TIM2->ARR = 233; /* (2) */
  TIM2->CCR1 = 0; //116; /* (3) */
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1
 | TIM_CCMR1_OC1PE; /* (4) */
  TIM2->CCER |= TIM_CCER_CC1E; /* (5) */
  TIM2->CR1 |= TIM_CR1_CEN; /* (6) */
  TIM2->EGR |= TIM_EGR_UG; /* (7) */
  
}
