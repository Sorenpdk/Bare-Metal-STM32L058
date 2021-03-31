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
#define HIGH     1
#define LOW      0
#define E_PULSE 13
#define RS      10 // Low = Instruction input High = Data input
#define WR      6  // Low = Write High = Read
#define DB4     0
#define DB5     1
#define DB6     2
#define DB7     3
#define STAR    0x2A
#define BLANK   0x20
#define UINT16  65536
#define TENS    9
#define HUNDREDS        99
#define THOUSANDS       999
#define TEN_THOUSANDS   9999


/* Private variables ---------------------------------------------------------*/
static char globalbuffer[16];

unsigned char ManFlatArms[] = { 
  0x0E, 
  0x0E, 
  0x04, 
  0x04, 
  0x1F, 
  0x04, 
  0x0A, 
  0x0A };


unsigned char ManRaiseArms[] = { 
  0x0E,
  0x0E,
  0x04,
  0x15,
  0x0E,
  0x04,
  0x0A,
  0x0A };

/* Private function prototypes -----------------------------------------------*/
void lcd_gpio(uint8_t pin, uint8_t pinState);
void lcd_pulse();
void lcd_clear_display();
void lcd_init();
void lcd_write_string(unsigned char *data);
void lcd_transmit(uint8_t data);
void lcd_set_ddram(uint8_t column, uint8_t row);
void lcd_write_cmd(uint8_t cmd);
void lcd_set_cgram(uint8_t cgram);
void lcd_write_data(uint8_t data);
void lcd_custom_char(unsigned char* bitmap, uint8_t cgrampos);
void check_placevalue_and_display(uint16_t localcycles);


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
  lcd_init();
  lcd_custom_char(ManFlatArms, 0);
  lcd_custom_char(ManRaiseArms, 1);
  lcd_clear_display();
  lcd_set_ddram(1,0);
  lcd_write_string("Cycles 0");
  
  static uint16_t cycles;
 
  while (1)
  {

    for(int i = 0; i < 10; i++){ // up to 16
      customDelay(30);
      lcd_set_ddram(0,i);
      lcd_write_data(STAR);
    }
    
    lcd_set_ddram(1,13);
    lcd_write_data(0);
  
    for(int i = 0; i < 10; i++){
      customDelay(30);
      lcd_set_ddram(0,i);
      lcd_write_data(BLANK);
    }
     
    cycles++;  
    sprintf(globalbuffer, "%d", cycles);   

    check_placevalue_and_display(cycles);

    lcd_set_ddram(1,13);
    lcd_write_data(1);
  
  }
  
  
}

void check_placevalue_and_display(uint16_t localcycles)
{
   lcd_set_ddram(1,7);
   lcd_write_data(globalbuffer[0]);
    
  if(localcycles > TENS){
      lcd_set_ddram(1,8);
      lcd_write_data(globalbuffer[1]); 
    }
  
    if(localcycles > HUNDREDS){
      lcd_set_ddram(1,9);
      lcd_write_data(globalbuffer[2]); 
    }
    
    if(localcycles > THOUSANDS){
      lcd_set_ddram(1,10);
      lcd_write_data(globalbuffer[3]); 
    }
    
    if(localcycles > TEN_THOUSANDS){
      lcd_set_ddram(1,11);
      lcd_write_data(globalbuffer[4]); 
    }
    
    /* Clear the cycle counter display if we roll over 16 unsigned bits */
    if(!(localcycles % UINT16)){
      for(uint8_t i = 7; i < 11; i++){
        lcd_set_ddram(1,i);
        lcd_write_data(BLANK);
      }
    }
  
}

void lcd_clear_display()
{ 
  lcd_gpio(RS, LOW); // Select Instruction mode
  lcd_gpio(WR, LOW); // Select Write mode
  lcd_transmit(0x01);
}

void lcd_custom_char(unsigned char* bitmap, uint8_t cgrampos)
{
  cgrampos &= 0x07;
  lcd_write_cmd(0x40 | (cgrampos << 3));
  
  for(uint8_t i = 0; i < 8; i++) // populate with bitmap, AC is incremented auto.
  {
    lcd_write_data(bitmap[i]); 
  }
  
}

void lcd_init()
{
  /******************** Wait for the LCD to boot and prepare init *************/
  customDelay(1500);
  lcd_gpio(RS, LOW); // Select Instruction mode
  lcd_gpio(WR, LOW); // Select Write mode
  

  /*********************** Function set (0x28) ********************************/ 
  // Start 4 bit mode
  lcd_gpio(DB4, LOW); // DL LOW - 4 bit mode
  lcd_gpio(DB5, 1);   // Mandatory
  lcd_gpio(DB6, LOW);
  lcd_gpio(DB7, LOW); 
  lcd_pulse();
  // Select Number of Lines (2) and Font Type (5x8)
  lcd_gpio(DB4, LOW);  // -
  lcd_gpio(DB5, LOW);  // -
  lcd_gpio(DB6, LOW);  // F - Display 5x8 dots
  lcd_gpio(DB7, HIGH); // N - 2 lines
  lcd_pulse();

  /************************* Entry mode set (0x06) ****************************/ 
  lcd_gpio(DB4, LOW);
  lcd_gpio(DB5, LOW); 
  lcd_gpio(DB6, LOW);
  lcd_gpio(DB7, LOW); 
  lcd_pulse();
  lcd_gpio(DB4, LOW);  // S/H
  lcd_gpio(DB5, HIGH); // I/D
  lcd_gpio(DB6, 1);    // Mandatory
  lcd_gpio(DB7, LOW); 
  lcd_pulse();

  /*************************** Display ON/OFF (0x0E) **************************/ 
  lcd_gpio(DB4, LOW);
  lcd_gpio(DB5, LOW); 
  lcd_gpio(DB6, LOW);
  lcd_gpio(DB7, LOW); 
  lcd_pulse();
  lcd_gpio(DB4, LOW);  // B - CURSOR BLINK ON / OFF 
  lcd_gpio(DB5, LOW); // C  - CURSOR ON / OFF
  lcd_gpio(DB6, HIGH); // D  - Display ON/OFF
  lcd_gpio(DB7, 1);    // Mandatory
  lcd_pulse();

  /************************* Clear display (0x01) *****************************/
  lcd_gpio(DB4, LOW); 
  lcd_gpio(DB5, LOW); 
  lcd_gpio(DB6, LOW); 
  lcd_gpio(DB7, LOW);
  lcd_pulse();

  lcd_gpio(DB4, 1);  // Mandatory
  lcd_pulse();
  
  /************************* Initialization Done ******************************/
  lcd_set_ddram(0, 0);
  lcd_write_string("Initialization");
  lcd_set_ddram(1, 0);
  lcd_write_string("Done...");
  
  customDelay(1000); // Give some time to show our message
}

void lcd_transmit(uint8_t data)
{
    static uint8_t LSB;
    static uint8_t MSB;

    // Get MSB
    MSB = (data & 0xF0) >> 4;
    // Get LSB
    LSB = (data & 0x0F);

    /* Transmit MSB */
    lcd_gpio(DB4, ((MSB >> 0)&0x01)); 
    lcd_gpio(DB5, ((MSB >> 1)&0x01));
    lcd_gpio(DB6, ((MSB >> 2)&0x01)); 
    lcd_gpio(DB7, ((MSB >> 3)&0x01));   
    lcd_pulse();
      
    /* Transmit LSB */
    lcd_gpio(DB4, ((LSB >> 0)&0x01));
    lcd_gpio(DB5, ((LSB >> 1)&0x01));
    lcd_gpio(DB6, ((LSB >> 2)&0x01));
    lcd_gpio(DB7, ((LSB >> 3)&0x01));
    lcd_pulse();
}



void lcd_set_ddram(uint8_t column, uint8_t row)
{
    lcd_gpio(RS, LOW); // Select Instruction mode
    lcd_gpio(WR, LOW); // Select Write mode
     
    if(column){ // if second line
      if(row <= 15){
        lcd_transmit(row | 0xC0);  
      }
      // TODO else update error handler and display.
    }
    
    else{
      if(row <= 15){
        lcd_transmit(row | 0x80);
      }
      // TODO else update error handler and display.
    }
}

void lcd_write_cmd(uint8_t cmd)
{
  lcd_gpio(RS, LOW); // Select Instruction mode
  lcd_gpio(WR, LOW); // Select Write mode
  lcd_transmit(cmd); 
  
}

void lcd_set_cgram(uint8_t cgramaddr)
{
  lcd_write_cmd((cgramaddr | 0x40) * 8 );
  
}

void lcd_write_data(uint8_t data)
{
  lcd_gpio(RS, HIGH); // Select Data mode
  lcd_gpio(WR, LOW); // Select Write mode
  lcd_transmit(data); 
  
}

void lcd_write_string(unsigned char *data)
{
  while(*data){
    lcd_write_data(*data++);
  }
  
}


void lcd_gpio(uint8_t pin, uint8_t pinState) // TODO should add port as argument so we can switch between.
{

    if(pinState){  
      GPIOC->BSRR = 1UL << pin;
    }
    
    else{
      GPIOC->BSRR = 1UL << (pin + 16);
    } 
    
  
}

void lcd_pulse()
{
  lcd_gpio(E_PULSE, HIGH);
  customDelay(10);
  lcd_gpio(E_PULSE, LOW);
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

