/** remember to uncomment/comment the default implementation of systick handler inside stm32l0xx_it.c **/


/* Includes ------------------------------------------------------------------*/
#include "customdelay.h"

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t Tick;

/** This should be called _AFTER_ system clock configurations  **/
void customDelay_init(void)
{
  /** Sets the reload register SysTick->LOAD, 
      Set Priority for Systick Interrupt,
      Load the SysTick Counter Value SysTick->VAL
      congifures SysTick->CTRL Enable SysTick IRQ and SysTick Timer  **/
  
   SysTick_Config(SystemCoreClock/1000); /* 1ms config */
}


void customDelay(uint32_t ms)
{
  Tick = 0;
  while(Tick < ms);
}


/******************************************************************************/
/*            Cortex-M0 Plus Processor Exceptions Handlers                    */
/******************************************************************************/

/**
  * Brief   This function implements the SysTick handler,
  *         which increments the variable Tick every millisecond.
  *         
  *         
  * Param   None
  * Retval  None
  */
void SysTick_Handler(void)
{
  Tick++;
  
}


