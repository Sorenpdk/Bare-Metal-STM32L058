*******************************************************************************
*  				Lessons Learned        
*******************************************************************************
Fell into the STM32 nucleo board trap again, with predetermined pins, this time Port C pin 14-15 was used for 
from the data sheet:
Oscillator from external PC14: from external oscillator through the pin 25 of CN7
connector.
The following configuration is needed:
– SB48 and SB49 ON
– R34 and R36 removed
• LSE not used: PC14 and PC15 are used as GPIOs instead of low-speed clocks.
The following configuration is needed:
– SB48 and SB49 ON
– R34 and R36 removed

So PC14/PC15 is reserved for LSE and cannot be used as GPIO directly

There are a lot of timing issues to consider when working with LCD's
The power supply should be decoupled ofcourse, as the LCD connected directly to the MCU via PC USB is sporadic.
If left alone over a weekend it works fine, but if you plug in your PC it will corrupt. 


LCD 1602A
--------- Pins ---------
BL+ : Backlight VDD (BLA)
BL- : Backlight GND (BLK)
D0 to D7: Data pins (DB0 - DB7)
You can either choose an 8 bit mode which use all 8 inputs (DB0-DB7)
a regular command follows a full single byte e.g. 0b0000 0011

Pros: You can perform an instruction in a single write cycle.
Cons: You will need 8 dedicated I/O'select

Or you can either choose a 4 bit mode which use 4 inputs (DB4-DB7)
a regular command follows a split of a single byte e.g.  0b0000 0011
First the higher four nibbles of a character are sent to the lcd with an enable stroke. 
Than the lower four nibbles are send with enable stroke.

#include <stdio.h>

#include <stdint.h>


int main()

{
     
uint8_t i = 0xA4; // 0b 1010 0100  164DEC  
    
uint8_t LSB = (i & 0x0F);
    
uint8_t MSB = ((i >> 4 ) & 0x0F);
    
printf("%d\r\n",LSB);
    
printf("%d\r\n",MSB);
    
return 0;

}
Pros: You will save 4 GPIOs
Cons: In 4-bit mode character is displayed on lcd in two pulse signals. 
Since two pulse (enable) signals are required to display a single character so 4-bit mode latency time is high.

E : Enable signal
RS : Register select - Selection of whether you want to send an instruction(W) or data(R/W)
R/W : Classic Read / Write
Say you want to send an instruction you will select RS = 0 and R/W = 0

Alternatively you can interface the LCD controller with an I2C module and use the full 8 bit mode with only 4 wires (VDD GND SDA SCL)

https://www.engineersgarage.com/knowledge_share/lcd-in-4-bit-mode-and-8-bit-mode/

--------- Communication ---------

--------- Other ---------

--------- Clock / registers ---------


--------- CM0+ / STM32 --------- 


--------- Protocol ---------


--------- Software / Basic operations ---------




--------- IAR ---------
/CMSIS contains all core files
- CortexM0+ core <core_cm0plus.h>
- CortexMFunctions <core_cmFunc.h>
- CortexMInstructions <core_cmInstr>

/EWARM contains all project files
- MCU startup assembly instructions <startup_stm32l053xx.s>
- Linker file for flash <stm32l053xx_flash.icf>
- Linker file for SRAM <stm32l053xx_sram>


/Inc contains all include files MCU specific
- stm32l0xx.h
- stm32l0xx_it.h
- stm32l053xx.h
- system_stm32l0xx.h

/Src contains all source files MCU specific
- stm32l0xx_it.c
- system_stm32l0xx

/User contains application code
- main.c


