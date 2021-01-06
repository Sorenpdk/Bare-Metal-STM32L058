/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *            of GPIO toggling, specifically the toggling of user LED (LD2) 
  *            via GPIO PA5 on the Nucleo-L053R8 STM32 development board.
  *          
  ******************************************************************************
 */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l053xx.h"

/* Private define ------------------------------------------------------------*/
#define RESET_VAL_PORTA_MODER 0xEBFFFCFF
#define RESET_VAL_PORTA_PUPDR 0x24000000
#define RESET_VAL_PORTA_SPEEDR 0x0C000000 

/* Private function prototypes -----------------------------------------------*/
void  ConfigureGPIO(void);

/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
 
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l053xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l0xx.c file
     */
  
 ConfigureGPIO();

  while (1)
  {
    // Atomic write - Set [Bit 5] [Val 1] to turn ON PA5 (Port A Pin 5 (LD2))
    GPIOA->BSRR = (1<<5); 
    
    // Atomic write - Set [Bit 21] [Val 1] to turn OFF PA5
    GPIOA->BSRR = (1<<21);   
  }
}

/**
  * Brief   This function enables the peripheral clocks on GPIO port A,
  *         configures GPIO PA5 in output mode push-pull enables internal pull-up 
  *         and sets the GPIO speed to medium.
  * Param   None
  * Retval  None
  */
__INLINE void ConfigureGPIO(void)
{  
  /* (1) Enable the peripheral clock of GPIOA[Bit 0] [Val 1]*/
  /* (2) Select General purpose output mode [Bit 11:10] [Val 0:1] on GPIOA pin 5 */
  /* (3) Select Output type push-pull mode */
  /* (4) Select Pull-up (10) for PA5 */
  /* (5) Select Medium speed [Bit 11:10] [Val 0:1] for PA5 */
  
  RCC->IOPENR |= 0x1;  /* (1) */ 
  
  GPIOA->MODER = (GPIOA->MODER & RESET_VAL_PORTA_MODER) | (1<<10); /* (2) */
  GPIOA->MODER = (GPIOA->MODER & RESET_VAL_PORTA_MODER) & ~(1<<11); /* (2) */
 
  GPIOA->OTYPER &= ~(1<<5);  /* (3) */
  
  GPIOA->PUPDR = (GPIOA->PUPDR & RESET_VAL_PORTA_PUPDR) | 0x00000400;  /* (4) */
 
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & RESET_VAL_PORTA_SPEEDR) | 0x00000400;   /* (5) */
}


/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

