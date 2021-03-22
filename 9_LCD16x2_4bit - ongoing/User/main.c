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
#include "stm32l053xx.h"
#include "customclock.h"
#include "customgpio.h"
#include "customdelay.h"
/* Private define ------------------------------------------------------------*/

#define A 0x41 //0100 0001

#define HIGH 1
#define LOW 0
#define E_PULSE 13
#define RS 10 // Low = Instruction input High = Data input
#define WR 6  // Low = Write High = Read
#define DB4 0
#define DB5 1
#define DB6 2
#define DB7 3

/*

 uint8_t i = 0xA4; // 0b 1010 0100  164DEC  
    uint8_t LSB = (i & 0x0F);
    uint8_t MSB = ((i & 0xF0) >> 4 );

*/



// PC 15 issues
// PC 14 issues PC 10 issues

/* Private function prototypes -----------------------------------------------*/
void LCD_GPIO(uint8_t pin, uint8_t pinState);
void LCD_Init();
void pulse();
void clear();



/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
  customClock_init();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  customGPIO_init();
  customDelay_init();
  LCD_Init();


  while (1)
  {
  
  
  }
}


void LCD_Init()
{
   customDelay(1000);
  LCD_GPIO(RS, LOW); // Instruction
  LCD_GPIO(WR, LOW); // Write
  
  // 4 bit mode
   LCD_GPIO(DB4, LOW); // Instruction
   LCD_GPIO(DB5, HIGH); // Instruction
   LCD_GPIO(DB6, LOW); // Instruction
   LCD_GPIO(DB7, LOW); // Instruction
   pulse();
   
   // Clear display
   LCD_GPIO(DB4, LOW); // Instruction
   LCD_GPIO(DB5, LOW); // Instruction
   LCD_GPIO(DB6, LOW); // Instruction
   LCD_GPIO(DB7, LOW); // Instruction
   pulse();
  
    LCD_GPIO(DB4, HIGH); // Instruction
   pulse();
   
   // Return home
   LCD_GPIO(DB4, LOW); // Instruction
   LCD_GPIO(DB5, LOW); // Instruction
   LCD_GPIO(DB6, LOW); // Instruction
   LCD_GPIO(DB7, LOW); // Instruction
   pulse();
  
    LCD_GPIO(DB5, HIGH); // Instruction
   pulse();
   
   // Display on no cursor
   LCD_GPIO(DB4, LOW); // Instruction
   LCD_GPIO(DB5, LOW); // Instruction
   LCD_GPIO(DB6, LOW); // Instruction
   LCD_GPIO(DB7, LOW); // Instruction
   pulse();
  
    LCD_GPIO(DB6, HIGH); // Instruction
    LCD_GPIO(DB7, HIGH); // Instruction
   pulse();
   
   LCD_GPIO(RS, HIGH); // Instruction
   // Transmit H
   LCD_GPIO(DB4, LOW); // Instruction
   LCD_GPIO(DB5, LOW); // Instruction
   LCD_GPIO(DB6, HIGH); // Instruction
   LCD_GPIO(DB7, LOW); // Instruction
   pulse();
  
     LCD_GPIO(DB6, LOW); // Instruction
    LCD_GPIO(DB7, HIGH); // Instruction
   pulse();

}


void LCD_GPIO(uint8_t pin, uint8_t pinState)
{

    if(pinState)
    {  
      GPIOC->BSRR = 1UL << pin;
    }
    
    else
    {
      GPIOC->BSRR = 1UL << (pin + 16);
    } 
    
  
}

void clear()
{  
  LCD_GPIO(DB4, LOW); 
  LCD_GPIO(DB5, LOW); 
  LCD_GPIO(DB6, LOW);
  LCD_GPIO(DB7, LOW);
}
void pulse()
{
  LCD_GPIO(E_PULSE, HIGH);
  customDelay(100);
  LCD_GPIO(E_PULSE, LOW);
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

