/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *            Basic USART Rx example.
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

uint8_t i = 0;
uint8_t message[] = {0};

int main(void)
{
  customClock_init();
  SystemCoreClockUpdate();
  customDelay_init(); // SysTick_Config
  customGPIO_init();
  customUSART1_Init();
 
  // 1 enable interrupt 
  NVIC_EnableIRQ(USART1_IRQn);
  NVIC_SetPriority(USART1_IRQn,0);
  
  
  while (1)
  {  
    
  }
}

/**
  * Brief   This function handles USART1 interrupt request.
  * Param   None
  * Retval  None
  */
void USART1_IRQHandler(void)
{
 if((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
 {
   uint8_t message[] = "Data Received!\r\n";
   UART_Send_String(message,sizeof(message));
   uint8_t USART_Data = 0;
   USART_Data = UART_Receive_Char();  
   
  if((USART1->ISR & USART_ISR_TXE) == USART_ISR_TXE)
  {
    uint8_t message[] = "From TX: ";
    UART_Send_String(message,sizeof(message));
    UART_Send_Char(USART_Data);
    UART_Send_Char(0x0D); // CR
    UART_Send_Char(0x0A); // LF
  }
 }
   
}



/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

