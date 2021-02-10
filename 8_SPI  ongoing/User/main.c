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
#include "customdelay.h"
#include "customgpio.h"
#include "stdlib.h"

/* Private define ------------------------------------------------------------*/

#define BUFFER_SIZE     8
#define SPI_SET_LOW     0 
#define SPI_SET_HIGH    1                       
#define SPI_WP          13                      
#define SPI_HOLD        14                     
#define SPI_CS          4    

#define SPI_DUMMY_8_BIT         0x00
#define SPI_DUMMY_16_BIT        0x0000

/* Private function prototypes -----------------------------------------------*/

void SPI_GPIO(uint8_t pin, uint8_t pinState);
void SPI_init();
void SPI_Transmit(uint8_t *txData, uint8_t dataLen);

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
  SPI_init();
 
  
  uint8_t TX_Array[BUFFER_SIZE] = {0x53,0x50,0x49,0x48,0x45,0x4C,0x4C,0x4F};
  
  while (1)
  {  
    SPI_GPIO(SPI_CS,SPI_SET_LOW);
    SPI_Transmit(TX_Array, 8);
    SPI_GPIO(SPI_CS,SPI_SET_HIGH);
    
   
  }
}

void SPI_init()
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  SPI1->CR1 |= SPI_CR1_BIDIMODE; // Full duplex
  SPI1->CR1 |= SPI_CR1_BIDIOE; // Output enabled (transmit-only mode)
  
  SPI1->CR1 |= SPI_CR1_MSTR; 
  SPI1->CR1 |= SPI_CR1_BR_2; // fPCLK/32
  SPI1->CR2 |= SPI_CR2_SSOE;
  SPI1->CR1 |= SPI_CR1_SSM;
  SPI1->CR1 &= ~(SPI_CR1_CPHA | SPI_CR1_CPOL);

  SPI_GPIO(SPI_HOLD,SPI_SET_HIGH); // initial state high
  SPI_GPIO(SPI_WP,SPI_SET_HIGH); 
  SPI_GPIO(SPI_CS,SPI_SET_HIGH);
  
  SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI
  
} 
void SPI_Transmit(uint8_t *txData, uint8_t dataLen)
{

    for(uint8_t i = 0; i < dataLen; i++)
    {
    *(uint8_t *)&(SPI1->DR) = txData[i];
      while(!(SPI1->SR & SPI_SR_TXE));
      while(SPI1->SR & SPI_SR_BSY); 
    }
  }

void SPI_GPIO(uint8_t pin, uint8_t pinState)
{
  
  switch(pin)
  {
    case 4: // If Chip Select use port A
    if(pinState)
    {  
      GPIOA->BSRR = 1UL << pin;
    }
    
    else
    {
      GPIOA->BSRR = 1UL << (pin + 16);
    } 
    
  default:
    if(pinState)
    {  
      GPIOB->BSRR = 1UL << pin;
    }
    else
    {
      GPIOB->BSRR = 1UL << (pin + 16);
    } 
  }
}


/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

