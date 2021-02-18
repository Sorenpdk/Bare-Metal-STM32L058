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
#include <string.h> // memcpy

/* Private define ------------------------------------------------------------*/

#define BUFFER_SIZE     8
#define SPI_SET_LOW     0 
#define SPI_SET_HIGH    1                       
#define SPI_WP          13                      
#define SPI_HOLD        14                     
#define SPI_CS          4    

#define SPI_DUMMY_8_BIT         0x00
#define SPI_DUMMY_16_BIT        0x0000

#define MX25_WRSR_CMD 0x01 // (write status register) to write new status register
#define MX25_PAGEPROG_CMD 0x02 // (Page Program)to program the selected page
#define MX25_READ_CMD 0x03 // n bytes read out until CS# goes high
#define MX25_WRDI_CMD 0x04 // (write disable) reset the(WEL) write enable latch bit
#define MX25_RDSR_CMD 0x05 // (read status register) to read outthe status register
#define MX25_WREN_CMD 0x06 //(write Enable) sets the (WEL) write enable latch bit
#define MX25_SECTERASE_CMD 0x20 // (Sector Erase) to erase the selected sector
#define MX25_BLOCKERASE_CMD 0x52 // (Block Erase) to erase the selected block
#define MX25_CHIPERASE_CMD 0x60 // (Chip Erase) to erase whole chip

#define MX25_RES_CMD 0xAB // to read out 1-byte Device ID
#define MX25_REMS_CMD 0x90 // (Read Electronic Manufacturer & Device ID) Output the manufacturer ID and device ID
#define MX25_RDID_CMD 0x9f // (read identification) output the manufacturer ID and 2-byte device ID

/* Private function prototypes -----------------------------------------------*/

void SPI_GPIO(uint8_t pin, uint8_t pinState);
void SPI_init();

void SPI_TransmitReceive(uint8_t *txData, uint8_t *rxData, uint8_t dataLen);
void MX25L20_WRDI();
void MX25L20_WREN();
uint8_t MX25L20_RDID();
uint8_t MX25L20_RDSR();

uint8_t cmd_rx[BUFFER_SIZE] = {0};

int main(void)
{
  customClock_init();
  SystemCoreClockUpdate();
  customDelay_init(); // SysTick_Config
  customGPIO_init();
  SPI_init();
  
 
  while (1)
  { 
    MX25L20_RDID();
    customDelay(1);
    MX25L20_WREN();
    MX25L20_RDSR();
    MX25L20_WRDI();
    MX25L20_RDSR();
     
    customDelay(3000); // To avoid 1mil cycles to the flash
 
  }
}

void MX25L20_WRDI()
{
   static uint8_t cmd_tx[] = 
    {
      MX25_WRDI_CMD,   
    };
    
    SPI_GPIO(SPI_CS,SPI_SET_LOW); 
    SPI_TransmitReceive(cmd_tx, cmd_rx, 1);
    SPI_GPIO(SPI_CS,SPI_SET_HIGH);
  
}


uint8_t MX25L20_RDSR()
{
  static uint8_t cmd_tx[] = 
    {
      MX25_RDSR_CMD,
      SPI_DUMMY_8_BIT,
      SPI_DUMMY_8_BIT
    };
      
    // Check status register
    SPI_GPIO(SPI_CS,SPI_SET_LOW);
    SPI_TransmitReceive(cmd_tx, cmd_rx, 3);
    SPI_GPIO(SPI_CS,SPI_SET_HIGH);
    
    return 1;
}


void MX25L20_WREN()
{
    static uint8_t cmd_rx[BUFFER_SIZE];
    static uint8_t cmd_tx[] = 
    {
      MX25_WREN_CMD,
    };
   
    SPI_GPIO(SPI_CS,SPI_SET_LOW); 
    SPI_TransmitReceive(cmd_tx, cmd_rx, 1);
    SPI_GPIO(SPI_CS,SPI_SET_HIGH);
  
}

uint8_t MX25L20_RDID()
{
  
  static uint8_t cmd_tx[] = 
  {
   MX25_RDID_CMD,
   SPI_DUMMY_8_BIT,
   SPI_DUMMY_8_BIT,
   SPI_DUMMY_8_BIT   
  };
  
  SPI_GPIO(SPI_CS,SPI_SET_LOW);  
  SPI_TransmitReceive(cmd_tx, cmd_rx, sizeof(cmd_tx)/sizeof(cmd_tx[0]));
  SPI_GPIO(SPI_CS,SPI_SET_HIGH);
   
  return 1;
}

void SPI_TransmitReceive(uint8_t *txData, uint8_t *rxData, uint8_t dataLen)
{
    static uint8_t temp[BUFFER_SIZE];
    
    for(uint8_t i = 0; i < dataLen; i++)
    {
      while(!(SPI1->SR & SPI_SR_TXE));
      SPI1->DR = txData[i];
      
      while(!(SPI1->SR & SPI_SR_RXNE));
      temp[i-1] = SPI1->DR;
    }
    
    while(SPI1->SR & SPI_SR_BSY);
    
    memcpy(&rxData, &temp, BUFFER_SIZE * sizeof(uint8_t)); 
}



void SPI_init()
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  SPI1->CR1 |= SPI_CR1_BIDIOE; // Output enabled (transmit-only mode)
  
  SPI1->CR1 |= SPI_CR1_MSTR; 
  SPI1->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0; // fPCLK/32
  SPI1->CR2 |= SPI_CR2_SSOE;
  SPI1->CR1 |= SPI_CR1_SSM;
  SPI1->CR1 &= ~(SPI_CR1_CPHA | SPI_CR1_CPOL);

  SPI_GPIO(SPI_HOLD,SPI_SET_HIGH); // initial state high
  SPI_GPIO(SPI_WP,SPI_SET_HIGH); 
  SPI_GPIO(SPI_CS,SPI_SET_HIGH);
  
  SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI
  
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

