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

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Configure_GPIO_USART1(void);
void Configure_USART1(void);
void UART_Send_Char(unsigned char c);
void UART_Send_String(const char* msg);
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
  Configure_USART1();
  customGPIO_init();
  
  unsigned char onechar = 'c';
  const char* message = "hello world";

  while (1)
  {
  
    UART_Send_String(message); 
    toggleLED2();
    customDelay(1000);
   
  }
}
/**
  * Brief   This function 
  *         sends a single char to the USART1
  *         data transmit register, and waits for the transmission to complete
  *        
  * Param   None
  * Retval  None
  */
void UART_Send_Char(unsigned char c)
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
void UART_Send_String(const char* msg)
{
  while(*msg != '\0')
  {
    UART_Send_Char(*msg++);
  }
  
}
/**
  * Brief  This function configures the USART1 with a 115200 baud rate
  *     The UART frame is constructed as follows
  *     By default in USART1->CR1 and CR2:
  *     8 data bit, 1 start bit, 1 stop bit, no parity LSB.
  *        
  * Param   None
  * Retval  None
  */
__INLINE void Configure_USART1(void)
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
  /* (4) Enable USART1 transmitter */
  /* (5) Enable transmission complete interrrupt */
  
  RCC->APB2ENR |= 1UL << 14; /* (1) */
  
  RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL_0 | RCC_CCIPR_USART1SEL_1); /* (2) */ 
  
  USART1->BRR = 104;  /* (3) */
  
  USART1->CR1 |= USART_CR1_TE | USART_CR1_UE; /* (4) */
  
  USART1->CR1 |= USART_CR1_TCIE;/* (5) */

}


/** General Info during research 
The commonly used baud rates are 2400 , 4800, 9600, 19.200, 56000, 115200 bauds.

Baud rate generator
The baud rate generator generates a sampling signal whose frequency is exactly 16
times the UART's designated baud rate. To avoid creating a new clock domain and
violating the synchronous design principle, the sampling signal should function as
enable ticks rather than the clock signal to the UART receiver.
For the 19,200 baud rate, the sampling rate has to be 307,200 (i.e., 19,200*16) ticks per
second. Assuming that the system clock rate is 50 MHz(if using Basys2 FPGA kit), the
baud rate generator needs a mod-163 (i.e., counter = clock frequency/samplingfrequency = (5*10^6)/ 307,200) counter, in which a one-clock-cycle tick is asserted
once every 163 clock cycles.


UART receiver
The DBIT constant indicates the number of data bits, and the SB-TICK constant
indicates the number of ticks needed for the stop bits, which is 16, 24, and 32 for 1,
1.5, and 2 stop bits, respectively. DBIT and SB-TICK are assigned to 8 and 16 in this
design.
The chart follows oversampling and includes three major states, start, data, and stop,
which represent the processing of the start bit, data bits, and stop bit. The s-tick signal
is the enable tick from the baud rate generator and there are 16 ticks in a bit interval.
Note that the FSMD stays in the same state unless the s-tick signal is asserted. There
are two counters, represented by the s and n registers. The s register keeps track of the
number of sampling ticks and counts to 7 in the start state, to 15 in the data state, and
to SB-TICK in the stop state. The n register keeps track of the number of data bits
received in the data state. The retrieved bits are shifted into and reassembled in the b
register. A status signal, rx-done-tick, is included. It is asserted for one clock cycle after
the receiving process is completed.


UART Transmitter
The UART transmitter operation is similar to a receiver and it is essentially a shift
register that shifts out data bits at a specific rate. The rate can be controlled by oneclock-cycle enable ticks generated by the baud rate generator. Because no
oversampling is involved, the frequency of the ticks is 16 times slower than that of the
UART receiver. Instead of introducing a new counter, the UART transmitter usually
shares the baud rate generator of the UART receiver and uses an internal counter to
keep track of the number of enable ticks. A bit is shifted out every 16 enable ticks.


Oversampling
The most commonly used sampling rate is 16 times the baud rate, which means that
each serial bit is sampled 16 times. Assume that the communication uses N data bits
and M stop bits. The oversampling scheme works as follows:
1) Wait until the incoming signal becomes 0, the beginning of the start bit, and
then start the sampling tick counter.
2) When the counter reaches 7, the incoming signal reaches the middle point of the
start bit. Clear the counter to 0 and restart.
3) When the counter reaches 15, the incoming signal progresses for one bit and
reaches the middle of the first data bit. Retrieve its value, shift it into a register,
and restart the counter.
4) Repeat step 3 n-1 more times to retrieve the remaining data bits.
5) If the optional parity bit is used, repeat step 3 one time to obtain the parity bit.
6) Repeat step 3 n more times to obtain the stop bits.
The oversampling scheme performs the function of a clock signal. Instead of using the
rising edge to indicate when the input signal is valid, it utilizes sampling ticks to
estimate the middle point of each bit. While the receiver has no information about the
exact onset time of the start bit, the estimation can be off by at most &. The subsequent
data bit retrievals are off by at most & from the middle point as well. Because of the
oversampling, the baud rate can be only a small fraction of the system clock rate, and
thus this scheme is not appropriate for a high data rate **/


/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

