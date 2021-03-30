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

/* Private variables ---------------------------------------------------------*/
static char buffer[16];

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

unsigned char bitmap[] = { 
  0x0e, 
  0x0e, 
  0x04, 
  0x04, 
  0x1f, 
  0x04, 
  0x0a, 
  0x0a};
unsigned char bitmap2[] = { 
 0x0E,
  0x0E,
  0x04,
  0x15,
  0x0E,
  0x04,
  0x0A,
  0x0A};
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
  lcd_custom_char(bitmap, 0);
  lcd_custom_char(bitmap2, 1);
  static uint16_t cycles;
  lcd_clear_display();
  lcd_set_ddram(1,0);
  lcd_write_string("Cycles 0");
  
 
  while (1)
  {
    
  
    
    for(int i = 0; i < 10; i++){ // up to 16
      customDelay(30);
      lcd_set_ddram(0,i);
      lcd_write_data(0x2A);
    }
    
    lcd_set_ddram(1,13);
    lcd_write_data(0);
  
    for(int i = 0; i < 10; i++){
      customDelay(30);
      lcd_set_ddram(0,i);
      lcd_write_data(0x20);
    }
     
  
    cycles++;
  
    sprintf(buffer, "%d", cycles);   
  
      lcd_set_ddram(1,7);
      lcd_write_data(buffer[0]);
  
    if(cycles > 9){
      lcd_set_ddram(1,8);
      lcd_write_data(buffer[1]); 
    }
  
    if(cycles > 99){
      lcd_set_ddram(1,9);
      lcd_write_data(buffer[2]); 
    }
    
    if(cycles > 999){
      lcd_set_ddram(1,10);
      lcd_write_data(buffer[3]); 
    }
    
    if(cycles > 9999){
      lcd_set_ddram(1,11);
      lcd_write_data(buffer[4]); 
    }
    
    /* Clear the cycle counter if we roll over */
    if(!(cycles % 65536)){
      for(uint8_t i = 7; i < 11; i++){
        lcd_set_ddram(1,i);
        lcd_write_data((uint8_t)' ');
      }
    }
      lcd_set_ddram(1,13);
      lcd_write_data(1);
  
  }
  
  
}


void lcd_clear_display() // fine
{ 
  lcd_gpio(RS, LOW); // Select Instruction mode
  lcd_gpio(WR, LOW); // Select Write mode
  lcd_transmit(0x01);
}

void lcd_custom_char(unsigned char* bitmap, uint8_t cgrampos)
{
  cgrampos &= 0x07; // set CGRAM address
  lcd_write_cmd(0x40 | (cgrampos << 3));
  for(uint8_t i = 0; i < 8; i++) // populate with bitmap, AC is incremented auto.
  {
    lcd_write_data(bitmap[i]);
    
  }
  
}

void lcd_init() // fine
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

  lcd_gpio(DB4, HIGH); 
  lcd_pulse();
  
  /************************* Initialization Done ******************************/
  lcd_set_ddram(0, 0);
  lcd_write_string("Initialization");
  

  lcd_set_ddram(1, 0);
  lcd_write_string("Done...");
  customDelay(1000); // Important delay as the very next instruction in main is clear display again
}

void lcd_transmit(uint8_t data) // fine
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

void lcd_set_ddram(uint8_t column, uint8_t row) // fine
{
    lcd_gpio(RS, LOW); // Select Instruction mode
    lcd_gpio(WR, LOW); // Select Write mode
     
    /** RS | R/W | DB7 | DB6 | DB5 | DB4 | DB3 | DB2 | DB1 | DB0  **/
    /** 0  |  0  | 1   | x   | x   | x   | x   | x   | x   | x  **/
    /** Data sheet says range for 1st line is 00 to 0x27 + the 7th bit = 0x80 for place 0 **/
    if(column){ // if second line
      if(row <= 15)
      lcd_transmit(row | 0xC0);  
    }   
    else{
      if(row <= 15)
      lcd_transmit(row | 0x80);
    }
}

void lcd_write_cmd(uint8_t cmd) // fine
{
  lcd_gpio(RS, LOW); // Select Instruction mode
  lcd_gpio(WR, LOW); // Select Write mode
  lcd_transmit(cmd); 
  
}

void lcd_set_cgram(uint8_t cgramaddr)
{
  lcd_write_cmd((cgramaddr | 0x40) * 8 );
  
}

void lcd_write_data(uint8_t data) // fine
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


void lcd_gpio(uint8_t pin, uint8_t pinState) // fine
{

    if(pinState){  
      GPIOC->BSRR = 1UL << pin;
    }
    
    else{
      GPIOC->BSRR = 1UL << (pin + 16);
    } 
    
  
}

void lcd_pulse() // fine
{
  lcd_gpio(E_PULSE, HIGH);
  customDelay(10);
  lcd_gpio(E_PULSE, LOW);
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

