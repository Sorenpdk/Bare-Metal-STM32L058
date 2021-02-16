*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

The USART2 interface available on PA2 and PA3 of the STM32 microcontroller can be
connected to ST-LINK MCU, ST morpho connector, or to ARDUINO® connector. The choice
can be changed by setting the related solder bridges.
By default, the USART2 communication between the target STM32 and ST-LINK MCU is enabled.
If the communication between the target STM32 PA2 (D1) or PA3 (D0) and shield or extension
board is required, SB62 and SB63 must be ON, while SB13 and SB14 must be OFF
See BoardGOTCHA.jpg
------------------------------------------------------------------------------------------
I did something to mess up the MCU .. changed GPIO ports from PA2-3 to use PA13-14-15 for GPIO.
The MCU wouldn't connect to ST LINK, holding the reset button forced it to connect.
The MCU won't halt when inserting break points.

Flashing another project(6_ButtonLED_EXTI) fixed the issue, so something is up with my SPI GPIO initialization...
I commented it all out and it the project works as expected.

While writing this it hit me that I am an idiot that should quit this ... Back in my very first project I found out PA13-PA14 was being used by SWD

From project 1_BlinkyLED2 :
 Some registers have a reset value - This is to ensure functionalities such as SWD/JTAG is always enabled.
* PA13 and PA14 is used for SWDIO SWDCLK, therefor specific reset values for PORTA in GPIOA->MODER and GPIOA->PUPDR and GPIA->SPEEDR etc.

===================================================================================
>>>>>>>>>>>>>>>>    NOTE TO SELF: Stay off PORT A !!     <<<<<<<<<<<<<<<<<<<
===================================================================================
2 hours lost on gotchas :( digging through initialization code, datasheet and reference manual for the MCU and the NUCLEO board documentation.

Switching the GPIOs to PB13-14-15 works as expected.
See GPIOTest_CS_WP_HOLD.jpg





Master mode fault (MODF)
Mode fault occurs when the master device has its internal SS signal (SS pin in SS hardware
mode, or SSI bit in SS software mode) pulled low. [...]
If you don't intend to use the NSS pin (N there stands for Negated), set both SPI_CFG2.SSM and SPI_CR1.SSI.


SPI clock only shows up when we transmit something

Configure the serial clock baud rate using the BR[2:0] bits 

Configure the CPOL and CPHA bits combination to define one of the four
relationships between the data transfer and the serial clock. (Note: 2 - except the
case when CRC is enabled at TI mode). Remember to check CPOL and CPHA supported by the slave.

Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
BIDIOE (RXONLY and BIDIMODE can't be set at the same time).
--------------------------------------------------------------------------------------
Why do we need to set SSI bit with SSM?
What is the purpose of SSOE bit?

It's related to the rarely used multi-master communication.
In a multi-master setup, the NSS signal controls access to the SPI bus. The ST documentation is unfortunately a bit vague there, but my understanding is that
NSS high input means the bus is free, you are allowed to transmit
NSS low input means someone else is transmitting, you become a slave.
Why do we need to set SSI bit with SSM?

If the SSM (Software Slave Management) bit is set in master mode, then the SSI (Slave Select Internal) bit becomes the source of the NSS signal instead of the pin. Setting SSI to 1 allows the master to transmit, setting it to 0 makes it a slave (clears the MSTR bit in CR1).

If you have a single master, just set

SPI->CR1 = SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_SSM | SPI_CR1_SSI
and don't worry about the rest. It's the most flexible way, you can control as many slaves as you like with GPIO outputs connected to the CS lines separately. You can use the NSS pin as GPIO as well.

What is the purpose of SSOE bit?

It changes the NSS pin to an output. Initially set to high, it becomes low when the controller starts transmitting (when the DR register is written to). Note that it won't automatically become high again when the transfer is finished, but by setting SPI_CR1_SPE to 0.

Using SSOE can be useful when a single master is talking to a single slave, because CS is controlled by the SPI registers. Not having to talk to a GPIO peripheral at all, there is no need to load its base address to a register and holding it there, saving some cycles and a couple of bytes in flash, making it possible to use a register for something else by an optimizing compiler.


----------------------------------------------------------------------------
Had some issues with timing, the CS would go high to soon, and we would miss the last byte when transmitting..
Inserting delays or making the data length 9 instead of 8 fixed the problem, but this is not very elegant, the data length + 1 approach is 100 % a poor choice, the delay approach chokes the transmission speed.

checking the SPI_BSY flag was the best solution.

-----------------------------------------------------------------------------------------------
/**
Data received or to be transmitted.
The data register is split into 2 buffers - one for writing (Transmit Buffer) and another one for
reading (Receive buffer). A write to the data register will write into the Tx buffer and a read
from the data register will return the value held in the Rx buffer.
For an 8-bit data frame, the buffers are 8-bit and only the LSB of the register
(SPI_DR[7:0]) is used for transmission/reception. 
When in reception mode, the MSB of the register (SPI_DR[15:8]) is forced to 0.
For a 16-bit data frame, the buffers are 16-bit and the entire register, SPI_DR[15:0] is
used for transmission/reception.
**/
     // Tell the compiler to force this the register to be 8 bit wide
     // take the address of the dataregister, cast that into a byte pointer and take the value of that pointer.
    	
	*(volatile uint8_t *)&(SPI1->DR) = txData[i];

The dissasembly code is the same as;

			SPI1->DR = txData[i];

The compiler is smart enough to know this is a single byte .. I don't know if its a mistake to not enforce the single byte.
from a portability aspect, I can see the reason to be very explicit.
-----------------------------------------------------------------------------------------------------------------------------------

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


