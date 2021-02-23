/**
  ******************************************************************************
  * @file    main.c
  * @author  Søren Pørksen
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *            
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
   

/* Error codes used to make the red led blinking */
#define ERROR_OVERFLOW 0x01
#define ERROR_UNEXPECTED_IT 0x02
/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
    
volatile uint32_t Period = 0; // Period of the signal applied on TI1 based on 48MHz clock
volatile uint32_t DutyCycle = 0; // High signal time based on 48MHz clock
volatile uint32_t uwDutyCycle = 0;
volatile uint32_t uwFrequency = 0;
volatile uint16_t error = 0xFF;  //initialized at 0 and modified by the functions 


void PWM_Init();
 
int main (void)
{
  customClock_init();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  customGPIO_init();
  PWM_Init();
  
  error = 0;
  
  while(1)
  { 
    
    
  }
  
}


void PWM_Init()
{
  /* Configure NVIC for TIMx */
  /* (1) Enable Interrupt on TIMx */
  /* (2) Set priority for TIMx*/
  NVIC_EnableIRQ(TIM21_IRQn); /* (1) */
  NVIC_SetPriority(TIM21_IRQn,0); /* (2) */
  
    /* (1) Select the active input TI1 for TIMx_CCR1 (CC1S = 01), 
         select the active input TI1 for TIMx_CCR2 (CC2S = 10) */ 
  /* (2) Select TI1FP1 as valid trigger input (TS = 101)
         configure the slave mode in reset mode (SMS = 100) */
  /* (3) Enable capture by setting CC1E and CC2E 
         select the rising edge on CC1 and CC1N (CC1P = 0 and CC1NP = 0, reset value),
         select the falling edge on CC2 (CC2P = 1). */
  /* (4) Enable interrupt on Capture/Compare 1 */
  /* (5) Enable counter */  
  
  TIM21->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_1; /* (1)*/
  TIM21->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 \
              | TIM_SMCR_SMS_2; /* (2) */
  TIM21->PSC = 12;
  TIM21->ARR = 0xffff-1;
  TIM21->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC2P; /* (3) */  
  TIM21->DIER |= TIM_DIER_CC1IE | TIM_DIER_UIE; /* (4) */
  TIM21->CR1 |= TIM_CR1_CEN; /* (5) */
  
 
   
}


void TIM21_IRQHandler(void)
{
if ((TIM21->SR & (TIM_SR_CC1IF | TIM_SR_CC1OF) ) == TIM_SR_CC1IF)
  {
    if (TIM21->CCR1 != 0)
    {
      Period = TIM21->CCR1;
      DutyCycle = TIM21->CCR2;
      uwDutyCycle = ((TIM21->CCR2 + 1) * 100) / (TIM21->CCR1 + 1);
      uwFrequency = (187500 / TIM21->PSC) / (TIM21->CCR1);
      error = 0;
    }
  }
  else if ((TIM21->SR & TIM_SR_CC1OF) != 0)  /* Check the overflow */
  {
    error = ERROR_OVERFLOW;
    TIM21->SR = ~(TIM_SR_CC1OF | TIM_SR_CC1IF); /* Clear the flags */
    return;
  }

  else
  {
    error = ERROR_UNEXPECTED_IT; /* Report an error */
  }
}




/*
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    // Get the Input Capture value 
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    
    if (uwIC2Value != 0)
    {
      // Duty cycle computation 
      uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + 1) * 100) / (uwIC2Value + 1);
      
      // uwFrequency computation
      // TIM2 counter clock = RCC_Clocks.HCLK_Frequency     
      uwFrequency = HAL_RCC_GetHCLKFreq()/ (uwIC2Value + 1);
    }
    else
    {
      uwDutyCycle = 0;
      uwFrequency = 0;
    }
  }
}
*/