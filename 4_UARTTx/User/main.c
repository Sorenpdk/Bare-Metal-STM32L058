/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *            Basic USART Tx example.
  *            
  *          
  ******************************************************************************
 */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include "customclock.h"
#include "customgpio.h"
#include "customuart.h"

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
  customDelay_init(); // SysTick_Config
  customGPIO_init();
  customUART_init();
  
  unsigned char onechar = 'c';
  const char* message = "hello world\r\n";

  while (1)
  {
  
    UART_Send_String(message); 
    toggleLED2();
    customDelay(1000);
   
  }
}




/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

