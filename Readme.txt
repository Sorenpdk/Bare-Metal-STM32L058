*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

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

    uint8_t i = 0xA4; // 0b 1010 0100  164DEC  
    uint8_t LSB = (i & 0x0F);
    uint8_t MSB = ((i & 0xF0) >> 4 );
   
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


