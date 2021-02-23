/* Includes ------------------------------------------------------------------*/
#include "customuart.h"


/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * Brief  This function configures the USART1 with a 115200 baud rate
  *     The UART frame is constructed as follows
  *     By default in USART1->CR1 and CR2:
  *     8 data bit, 1 start bit, 1 stop bit, no parity LSB.
  *        
  * Param   None
  * Retval  None
  */
void customUSART1_Init(void)
{
  /******************** INFO ****************************/
  /* USART1_TX is mapped to PA9 as alternate function */
  /* SYS_CLK is a 24MHz PLL derived from the 16MHz HSI */ 
  /* The AHB_CLK is the 24 MHz SYS_CLK prescaled 2 -> 12MHz*/  
  /* The APB1 and APB2 are derived from the AHB and not prescaled*/
  /* Such that APB2 -> 12 Mhz*/
  /* in USART1->CR1 if OVER8 = 0, BRR = USARTDIV */
  /* 1 stop bit is selected by default in USART1->CR2*/
  /* Tx/Rx baud = FCK / USARTDIV ->>> 115200 = ( 12*10^6 / x ) ->> x = 104d */
  /* Tx/Rx baud = FCK / USARTDIV ->>> 9600 = ( 12*10^6 / x ) ->> x = 1250d */
  /******************************************************/
  
  /* (1) Enable the peripheral clock for USART1 */
  /* (2) Select the APB clock as the USART1 clock */
  /* (3) oversampling by 16, 115200 baud */
  /* (4) Enable USART1 and transmitter, receiver */
  /* (5) Enable transmission complete, and on receive interrrupt */
  
  RCC->APB2ENR |= 1UL << 14; /* (1) */
  
  RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL_0 | RCC_CCIPR_USART1SEL_1); /* (2) */ 
  
  USART1->BRR = 104;  /* (3) */
  
  USART1->CR1 |=  USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; /* (4) */
   
  USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_TCIE;/* (5) */

}

/**
  * Brief   This function 
  *         sends a single char to the USART1
  *         data transmit register, and waits for the transmission to complete
  *        
  * Param   None
  * Retval  None
  */
uint8_t UART_Receive_Char()
{
 
   static uint8_t chartoreceive;
  
  if((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
    chartoreceive = (uint8_t)(USART1->RDR); /* Receive data, clear RXNE flag */
   
  } 
   return chartoreceive;   
}

/**
  * Brief   This function 
  *         sends a single char to the USART1
  *         data transmit register, and waits for the transmission to complete
  *        
  * Param   None
  * Retval  None
  */
void UART_Send_Char(uint8_t c)
{
 
      USART1->TDR = c;
     
      while(!(USART1->ISR & USART_ISR_TC))
      {
        /** 
          * Spin in here while we wait for the transmission to complete
          * The TC bit is set by hardware if the transmission of a 
          * frame containing data is complete and if TXE is set 
          */
        
      }
        
}


/**
  * Brief   This function 
  *         sends a string of chars to the USART1, until it hits the null terminator,
  *         data transmit register using a callback to UART_Send_Char();
  *         TODO: Implement some robustness for this function.
  * Param   None
  * Retval  None
  */
void UART_Send_String(uint8_t msg[], size_t len)
{
  for(uint8_t i = 0; i < len; i++){
    UART_Send_Char(msg[i]);
  }
  
}