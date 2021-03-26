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

#include "stdio.h"
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

// PC 15 issues
// PC 14 issues PC 10 issues
static char buffer[16];

/* Private function prototypes -----------------------------------------------*/
void LCD_GPIO(uint8_t pin, uint8_t pinState);
void pulse();
void LCD_transmit(uint8_t data);
void LCD_send_cmd(uint8_t data);
void LCD_Init();
void setcursor(uint8_t column, uint8_t row);

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
    
   LCD_GPIO(DB4, LOW); 
   LCD_GPIO(DB5, LOW); 
   LCD_GPIO(DB6, LOW); 
   LCD_GPIO(DB7, LOW);
   pulse();
   
   LCD_GPIO(DB4, HIGH); 
   pulse();
   
    setcursor(1,0);
  LCD_send_cmd((uint8_t)'C');
  LCD_send_cmd((uint8_t)'y');
  LCD_send_cmd((uint8_t)'c');
  LCD_send_cmd((uint8_t)'l');
  LCD_send_cmd((uint8_t)'e');
  LCD_send_cmd((uint8_t)'s');
  LCD_send_cmd((uint8_t)' ');
  LCD_send_cmd((uint8_t)'0');
  uint16_t cycles = 0;
  while (1)
  {
    
    customDelay(5);
   
  
  for(int i = 0; i < 16; i++)
  {
  
  customDelay(5);
  setcursor(0,i);
  LCD_send_cmd(0x2A);
  }
  
  for(int i = 0; i < 16; i++)
  {
  
  customDelay(5);
  setcursor(0,i);
  LCD_send_cmd(0x20);
  
  }
  cycles++;
  sprintf(buffer, "%d", cycles);   
  setcursor(1,7);
  LCD_send_cmd(buffer[0]);
  if(cycles > 9){
  setcursor(1,8);
 LCD_send_cmd(buffer[1]); 
  }
  if(cycles > 99)
  {
    setcursor(1,9);
     LCD_send_cmd(buffer[2]); 
  }
  
  if(cycles > 999)
  {
    setcursor(1,10);
     LCD_send_cmd(buffer[3]); 
  }
  
  if(cycles > 9999)
  {
    setcursor(1,11);
     LCD_send_cmd(buffer[4]); 
  }
  
  if(cycles == 65535)
  {
    setcursor(1,7);
     LCD_send_cmd((uint8_t)' ');
    setcursor(1,8);
    LCD_send_cmd((uint8_t)' ');
    setcursor(1,9);
    LCD_send_cmd((uint8_t)' ');
    setcursor(1,10);
    LCD_send_cmd((uint8_t)' ');
    setcursor(1,11);
    LCD_send_cmd((uint8_t)' ');
  }
  
  
    
   
  }
  
}


void LCD_Init()
{
  customDelay(1000);
  LCD_GPIO(RS, LOW); // Instruction mode
  LCD_GPIO(WR, LOW); // Write mode
  
  /* Function set */
  // Start 4 bit mode
   LCD_GPIO(DB4, LOW); // DL LOW - 4 bit mode
   LCD_GPIO(DB5, HIGH); 
   LCD_GPIO(DB6, LOW);
   LCD_GPIO(DB7, LOW); 
   pulse();
   // Select Number of Lines (2) and Font Type (5x11)
   LCD_GPIO(DB4, LOW); // -
   LCD_GPIO(DB5, LOW); // -
   LCD_GPIO(DB6, LOW); // F - Display 5x8 dots
   LCD_GPIO(DB7, HIGH); // N - 2 lines
   pulse();

 
   
   /* Entry mode set */  
   LCD_GPIO(DB4, LOW);
   LCD_GPIO(DB5, LOW); 
   LCD_GPIO(DB6, LOW);
   LCD_GPIO(DB7, LOW); 
   pulse();
   LCD_GPIO(DB4, LOW);  // S/H
   LCD_GPIO(DB5, HIGH); // I/D
   LCD_GPIO(DB6, 1);
   LCD_GPIO(DB7, LOW); 
   pulse();
   
 
   
   /* Display ON/OFF */
   LCD_GPIO(DB4, LOW);
   LCD_GPIO(DB5, LOW); 
   LCD_GPIO(DB6, LOW);
   LCD_GPIO(DB7, LOW); 
   pulse();
   LCD_GPIO(DB4, LOW);  // B - CURSOR BLINK ON / OFF 
   LCD_GPIO(DB5, HIGH); // C  - CURSOR ON / OFF
   LCD_GPIO(DB6, HIGH); // D  - Display ON/OFF
   LCD_GPIO(DB7, 1); 
   pulse();
   

    
   // Clear display
   LCD_GPIO(DB4, LOW); 
   LCD_GPIO(DB5, LOW); 
   LCD_GPIO(DB6, LOW); 
   LCD_GPIO(DB7, LOW);
   pulse();
   
   LCD_GPIO(DB4, HIGH); 
   pulse();
   //customDelay(2000); // important delay as the very next instruction is clear display again
  
   setcursor(0, 0);
  LCD_send_cmd((uint8_t)'I');
  LCD_send_cmd((uint8_t)'n');
  LCD_send_cmd((uint8_t)'i');
  LCD_send_cmd((uint8_t)'t');
  LCD_send_cmd((uint8_t)'i');
  LCD_send_cmd((uint8_t)'a');
  LCD_send_cmd((uint8_t)'l');
  LCD_send_cmd((uint8_t)'i');
  LCD_send_cmd((uint8_t)'z');
  LCD_send_cmd((uint8_t)'a');
  LCD_send_cmd((uint8_t)'t');
  LCD_send_cmd((uint8_t)'i');
  LCD_send_cmd((uint8_t)'o');
  LCD_send_cmd((uint8_t)'n');
  
   /*
  for(int i = 0; i < 26; i++)
   {
     LCD_send_cmd((uint8_t)' ');       
   }*/
  setcursor(1, 0);
  LCD_send_cmd((uint8_t)'D');
  LCD_send_cmd((uint8_t)'o');
  LCD_send_cmd((uint8_t)'n');
  LCD_send_cmd((uint8_t)'e');
  LCD_send_cmd((uint8_t)'.');
  LCD_send_cmd((uint8_t)'.');
  LCD_send_cmd((uint8_t)'.');
  
   LCD_GPIO(RS, LOW); // Instruction mode
   customDelay(2000); // important delay as the very next instruction is clear display again
}

void LCD_transmit(uint8_t data)
{
    uint8_t LSB = 0;
    uint8_t MSB = 0;

    // Get MSB
    MSB = (data & 0xF0) >> 4;
    // Get LSB
    LSB = (data & 0x0F);

    // populate and transmit MSB
    LCD_GPIO(DB4, ((MSB >> 0)&0x01)); 
    LCD_GPIO(DB5, ((MSB >> 1)&0x01));
    LCD_GPIO(DB6, ((MSB >> 2)&0x01)); 
    LCD_GPIO(DB7, ((MSB >> 3)&0x01));   
    pulse();
     customDelay(50);
     
    // populate and transmit LSB
    LCD_GPIO(DB4, ((LSB >> 0)&0x01));
    LCD_GPIO(DB5, ((LSB >> 1)&0x01));
    LCD_GPIO(DB6, ((LSB >> 2)&0x01));
    LCD_GPIO(DB7, ((LSB >> 3)&0x01));
    pulse();
}


void setcursor(uint8_t column, uint8_t row)
{
    LCD_GPIO(RS, LOW); // Instruction mode
    // SET DDRAM address
    
    // for 1 line (0x00 up to 0x0F)
    // 0100 xxxx
    if(column){
    LCD_transmit(row+0xC0);
    
    }
    else{
      LCD_transmit(row+0x80);
    }
}

void LCD_send_cmd(uint8_t data)
{
  LCD_GPIO(RS, HIGH); // Data mode
  LCD_transmit(data); 
  
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

void pulse()
{
  LCD_GPIO(E_PULSE, HIGH);
  customDelay(10);
  LCD_GPIO(E_PULSE, LOW);
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

