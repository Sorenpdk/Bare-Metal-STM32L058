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
#include "stm32l053xx.h"
#include "customclock.h"
#include "customgpio.h"
#include "customdelay.h"

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
 customClock_init();
 SystemCoreClockUpdate();
 SysTick_Config(SystemCoreClock/1000);
 
customGPIO_init();
customDelay_init();
//SysTick->CTRL = 0; /* Disable SysTick */ 

  RCC->APB2ENR |= 1UL << 0;
  EXTI->IMR |= 1UL<<13;
  EXTI->FTSR |= 1UL << 13; // enable falling edge detect
  NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn,0);
  SYSCFG->EXTICR[3] |= (uint16_t)SYSCFG_EXTICR4_EXTI13_PC;  // enable PC13 EXTI

  while (1)
  {
  
  }
}



void EXTI4_15_IRQHandler(void)
{
  if ((EXTI->PR & EXTI_PR_PR13) != 0)  /* Check line 0 has triggered the IT */
  toggleLED2();
  
  EXTI->PR |= EXTI_PR_PR13; /* Clear the pending bit */
  
}

/** 

User button1 PC13
EXTI13 bit 2
GPIO
All ports have external interrupt capability. To use external interrupt lines, the port must be
configured in input mode


SYSCFG external interrupt, input
if we trigger on rising edge (EXTI rising edge trigger selection register (EXTI_RTSR))
digital input

enable interrupt via NVIC_EnableIRQ(name)
select external source as interrupt in SYSCFG



look at STM32L-0 specific Interrupt Numbers in stm32l053xx.h for specific handler function look at vector table in start_up file**/





/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

