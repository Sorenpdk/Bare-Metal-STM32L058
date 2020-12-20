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
#include "customclock.h"
/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
 CustomClock_init();
  while (1)
  {
   
  }
}






/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

