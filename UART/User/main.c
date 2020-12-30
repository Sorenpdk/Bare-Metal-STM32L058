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
#include "customgpio.h"

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Configure_GPIO_USART1(void);
void Configure_USART1(void);
void UARTTX(uint8_t arr[]);
uint8_t counter = 0;
/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{

  uint8_t text[] = "hello world";
  customClock_init();
  SystemCoreClockUpdate();
  customDelay_init(); // SysTick_Config
  Configure_USART1();
  
  customGPIO_init();
  Configure_GPIO_USART1();
  
 
 
  while (1)
  {
   //toggleLED2();
  
   UARTTX(text);
   customDelay(1);
   
  }
}

void UARTTX(uint8_t arr[])
{
  
  /*
 Can use pointer variable, and while (char *ptr) .. this runs till we hit the NULLterminator.
then use USART1->TDR = *ptr++ to send.

We also have to wait for transmission to be completel
while(!TC) wait here till transfer complete is set (set by hardware)
*/
   
    
    // if so transmit the next character until we reach the end
    if(counter == 12)
    {
      counter = 0;
      USART1->ICR = USART_ICR_TCCF; /* Clear transfer complete flag */
    }
    
    else
    {
      USART1->TDR = arr[counter++];
      
    }
  
   //customDelay(5);
  
  
         
}


void Configure_USART1(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // 1: USART1 clock enabled
  // PERIPHERAL CLOCKS = AHB_PRES = 2 -> APB1 APB2 = 12 MHz 
  
  RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; // Select systemclk for usart
  
   /* Configure USART1 */
  /* (1) oversampling by 16, 9600 baud */
   // USART1->CR1 in When OVER8 = 0, BRR = USARTDIV
  /* (2) 8 data bit, 1 start bit, 1 stop bit, no parity LSB*/
   USART1->BRR = 2500;  /* (1) */
   
   //USART1->CR2 |= 00: 1 stop bit default
  
  // This bit can only be written when the USART is disabled (UE=0) 
   // Enable transmitter  and USART enable ---- 8-bit character length: M1(b28) and M0(b12) = 00 by default
  USART1->CR1 = USART_CR1_TE | USART_CR1_UE; /* (2) */
 
  //USART1->CR3 |= DMA 
  
  USART1->ICR = USART_ICR_TCCF;/* clear TC flag */
  USART1->CR1 |= USART_CR1_TCIE;/* enable TC interrupt */
  
  /** Receive and transmit data registers (USARTx_RDR, USARTx_TDR)
  A baud rate register (USARTx_BRR) 
The Transmit Enable bit (TE) must be set in order to activate the transmitter function. The
data in the transmit shift register is output on the TX pin and the corresponding clock pulses
are output on the SCLK pin

Tx/Rx baud = FCK / USARTDIV ->>> 9600 = ( 24*10^6 / x ) ->> x =  -> 2500d

How to derive USARTDIV from USARTx_BRR register values
Example 1
To obtain 9600 baud with fCK = 8 MHz.
• In case of oversampling by 16:
USARTDIV = 8 000 000/9600
BRR = USARTDIV = 833d = 0341h

• In case of oversampling by 8:
USARTDIV = 2 * 8 000 000/9600
USARTDIV = 1666,66 (1667d = 683h)
BRR[3:0] = 3h << 1 = 1h
BRR = 0x681
**/
}

/**
  * Brief   This function :
             - Enables GPIO clock
             - Configures the USART1 pins on GPIO PA9 PA10
  * Param   None
  * Retval  None
  */
__INLINE void Configure_GPIO_USART1(void)
{
  
  /* GPIO configuration for USART1 signals */
  /* (1) Select AF mode (10) on PA9 and PA10 */
  /* (2) AF4 for USART1 signals */
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE9|GPIO_MODER_MODE10))\
                 | (GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1); /* (1) */
   
        GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED10_1 | GPIO_OSPEEDER_OSPEED10_0;   /* (5) HIGH SPEED */
  GPIOA->AFR[1] = (GPIOA->AFR[1] &~ (0x00000FF0))\
                  | (4 << (1 * 4)) | (4 << (2 * 4)); /* (2) */
}

/**
The commonly used baud rates are 2400,4800, 9600, and 19,200 bauds.
**/
/**
Baud rate generator
The baud rate generator generates a sampling signal whose frequency is exactly 16
times the UART's designated baud rate. To avoid creating a new clock domain and
violating the synchronous design principle, the sampling signal should function as
enable ticks rather than the clock signal to the UART receiver.
For the 19,200 baud rate, the sampling rate has to be 307,200 (i.e., 19,200*16) ticks per
second. Assuming that the system clock rate is 50 MHz(if using Basys2 FPGA kit), the
baud rate generator needs a mod-163 (i.e., counter = clock frequency/samplingfrequency = (5*10^6)/ 307,200) counter, in which a one-clock-cycle tick is asserted
once every 163 clock cycles. **/

/**
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
the receiving process is completed. **/


/**
UART Transmitter
The UART transmitter operation is similar to a receiver and it is essentially a shift
register that shifts out data bits at a specific rate. The rate can be controlled by oneclock-cycle enable ticks generated by the baud rate generator. Because no
oversampling is involved, the frequency of the ticks is 16 times slower than that of the
UART receiver. Instead of introducing a new counter, the UART transmitter usually
shares the baud rate generator of the UART receiver and uses an internal counter to
keep track of the number of enable ticks. A bit is shifted out every 16 enable ticks. **/


/**
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

