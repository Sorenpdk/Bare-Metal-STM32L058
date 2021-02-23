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
#define RS 15 // Low = Instruction input High = Data input
#define WR 14  // Low = Write High = Read
#define DB4 0
#define DB5 1
#define DB6 2
#define DB7 3

/*

 uint8_t i = 0xA4; // 0b 1010 0100  164DEC  
    uint8_t LSB = (i & 0x0F);
    uint8_t MSB = ((i & 0xF0) >> 4 );

*/






/* Private function prototypes -----------------------------------------------*/
void LCD_GPIO(uint8_t pin, uint8_t pinState);
void lcd_put_cur(int row, int col);
void LCD_Init();
void lcd_clear(void);
void lcd_send_data(char data);
void lcd_send_string(char *str);
void send_to_lcd(char data, uint8_t rs);
void lcd_send_cmd(char cmd);
void pulse();
void clear();

int row = 0;
int col= 0;

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
    
    for(uint8_t i = 0; i < 128; i++)
    {
      lcd_put_cur(row,col);
      lcd_send_data(i+48);
      col++;
      if(col > 15)
      {
        row++;
        col = 0;
      }
      if(row > 1)
      {
        row = 0;
      }
      
    }
   
    
  }
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

void LCD_Init()
{
  LCD_GPIO(RS, LOW); // Instruction
  LCD_GPIO(WR, LOW); // Write
  
  clear();
 
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, HIGH); // enter 4 bit mode
  LCD_GPIO(DB6, LOW); // MSB
  LCD_GPIO(DB7, LOW); // MSB
  pulse();
  
  clear();
  
  // clear display
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, LOW); // MSB
  LCD_GPIO(DB6, LOW); // MSB
  LCD_GPIO(DB7, LOW); // MSB
  pulse();
  
  LCD_GPIO(DB4, HIGH); // MSB
  pulse();
  
  clear();
  
   // return home
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, LOW); // MSB
  LCD_GPIO(DB6, LOW); // MSB
  LCD_GPIO(DB7, LOW); // MSB
  pulse();
  
  LCD_GPIO(DB5, HIGH); // MSB
  pulse();
  
  clear();
  
     // Display on no cursor
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, LOW); // MSB
  LCD_GPIO(DB6, LOW); // MSB
  LCD_GPIO(DB7, LOW); // MSB
  pulse();
  LCD_GPIO(DB4, HIGH); // MSB
  LCD_GPIO(DB5, HIGH); // MSB
  LCD_GPIO(DB6, HIGH); // MSB
  LCD_GPIO(DB7, HIGH); // MSB
  pulse();
  
  clear();
  
   LCD_GPIO(RS, HIGH); // DATA
     // Transmit H
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, LOW); // MSB
  LCD_GPIO(DB6, HIGH); // MSB
  LCD_GPIO(DB7, LOW); // MSB
  pulse();
  LCD_GPIO(DB7, HIGH); // MSB
  pulse();
  clear();
  customDelay(100);
  
}

void clear()
{  
  LCD_GPIO(DB4, LOW); // MSB
  LCD_GPIO(DB5, LOW); // MSB
  LCD_GPIO(DB6, LOW); // MSB
  LCD_GPIO(DB7, LOW); // MSB
}
void pulse()
{
  LCD_GPIO(E_PULSE, LOW);
   customDelay(10);
  LCD_GPIO(E_PULSE, HIGH);
  customDelay(10);
  LCD_GPIO(E_PULSE, LOW);
  customDelay(20);
  
}

void lcd_put_cur(int row, int col)
{
  switch(row)
  {
  case 0:
    col |= 0x80;
    break;
  case 1:
    col |= 0xC0;
    break;
         
  }
  lcd_send_cmd(col);
}
void lcd_send_string(char *str)
{
  
  while(*str) lcd_send_data(*str++);
   
}


void send_to_lcd(char data, uint8_t rs)
{
  LCD_GPIO(RS, rs); // rs = 1 for data 0 for cmd.
  
  LCD_GPIO(DB7, ((data>>3)&0x01));
  LCD_GPIO(DB6, ((data>>2)&0x01));
  LCD_GPIO(DB5, ((data>>1)&0x01));
  LCD_GPIO(DB4, ((data>>0)&0x01));
  
  //Toggle EN pin
  LCD_GPIO(E_PULSE, HIGH);
  customDelay(10);
  LCD_GPIO(E_PULSE, LOW);
  customDelay(10);
}

void lcd_send_cmd(char cmd)
{
  
  char datatosend;
  /* Send upper nibble first */
  //datatosend = ((cmd>>4) & 0x0F);
  datatosend = cmd;
  send_to_lcd(datatosend,0);
  
  /* Send lower nibble 
  datatosend = ((cmd) & 0x0F);
  send_to_lcd(datatosend,0);
  */
}

void lcd_send_data(char data)
{
  
  char datatosend;
  /* Send upper nibble first */
  datatosend = ((data>>4) & 0x0F);
  send_to_lcd(datatosend,1);
  
  /* Send lower nibble */
  datatosend = ((data) & 0x0F);
  send_to_lcd(datatosend,1);
  
}

void lcd_clear(void)
{
  lcd_send_cmd(0x01);
  customDelay(2);
   
}



/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

