*******************************************************************************
*  				Lessons Learned        *
*******************************************************************************
------------------------------------------------------------------------------------------
Steps to follow
------------------------------------------------------------------------------------------
1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK and other pins, check clock requirements.
 - SPI is a totempole protocol, all outputs should be configured in push-pull mode.
 - It is a good idea to internally pull up lines like CS#, it can be good to put a pulldown (or pullup) on the MISO line. That is because this line is only driven by a slave when it's slave select line is asserted. When the bus is not used, all slave select lines are usually unasserted, so MISO floats. The arbitrary data value is irrelevant since nothing is looking at its value unless a slave is selected, but intermediate voltages on this line can cause the receiver in the master to draw excessive current or possibly oscillate.

It depends on how the SPI CLK output of the SPI master is configured by the firmware:
If it is configured as open drain output a pull-up resistor is needed.
If it is configured as push-pull output no pull-up resistor is needed


2. Write to the SPI_CR1 register:
a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 3).
b) Configure the CPOL and CPHA bits combination to define one of the four
 - Check slave requirements for polarity and phase
relationships between the data transfer and the serial clock. (Note: 2 - except the
case when CRC is enabled at TI mode).
c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
BIDIOE (RXONLY and BIDIMODE can't be set at the same time).
d) Configure the LSBFIRST bit to define the frame format (Note: 2).
e) Configure the CRCEN and CRCEN bits if CRC is needed (while SCK clock signal
is at idle state).
f) Configure SSM and SSI (Note: 2).
g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
NSS if master is configured to prevent MODF error).
h) Set the DFF bit to configure the data frame format (8 or 16 bits).
3. Write to SPI_CR2 register:
a) Configure SSOE (Note: 1 & 2).
b) Set the FRF bit if the TI protocol is required.
4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed.

It is recommended to enable the SPI slave before the master sends the clock. Otherwise,
undesired data transmission might occur.

Always always check flags set by hardware, such as TXE RXNE and SPI_BSY, this will help with a lot of timing issues.
while(!(SPI1->SR & SPI_SR_TXE));
      SPI1->DR = txData[i];
while(!(SPI1->SR & SPI_SR_RXNE));
      temp[i-1] = SPI1->DR;
while(SPI1->SR & SPI_SR_BSY);

Rx and Tx buffers
In reception, data are received and then stored into an internal Rx buffer while in
transmission, data are first stored into an internal Tx buffer before being transmitted. A read
access to the SPI_DR register returns the Rx buffered value whereas a write access to the
SPI_DR stores the written data into the Tx buffer.

Tx buffer handling
The data frame is loaded from the Tx buffer into the shift register during the first bit
transmission. Bits are then shifted out serially from the shift register to a dedicated output
pin depending on LSBFIRST bit setting.The TXE flag (Tx buffer empty) is set when the data
are transferred from the Tx buffer to the shift register. It indicates that the internal Tx buffer is
ready to be loaded with the next data. An interrupt can be generated if the TXEIE bit of the
SPI_CR2 register is set. Clearing the TXE bit is performed by writing to the SPI_DR register.
A continuous transmit stream can be achieved if the next data to be transmitted are stored
in the Tx buffer while previous frame transmission is still ongoing. When the software writes
to Tx buffer while the TXE flag is not set, the data waiting for transaction is overwritten.

Rx buffer handling
The RXNE flag (Rx buffer not empty) is set on the last sampling clock edge, when the data
are transferred from the shift register to the Rx buffer. It indicates that data are ready to be
read from the SPI_DR register. An interrupt can be generated if the RXNEIE bit in the
SPI_CR2 register is set. Clearing the RXNE bit is performed by reading the SPI_DR
register.
If a device has not cleared the RXNE bit resulting from the previous data byte transmitted,
an overrun condition occurs when the next value is buffered. The OVR bit is set and an
interrupt is generated if the ERRIE bit is set.
Another way to manage the data exchange is to use DMA (see Section 11.2: DMA main
features).

Sequence handling
The BSY bit is set when a current data frame transaction is ongoing. When the clock signal
runs continuously, the BSY flag remains set between data frames on the master side.
However, on the slave side, it becomes low for a minimum duration of one SPI clock cycle
between each data frame transfer.
For some configurations, the BSY flag can be used during the last data transfer to wait until
the completion of the transfer.
When a receive-only mode is configured on the master side, either in half-duplex
(BIDIMODE=1, BIDIOE=0) or simplex configuration (BIDIMODE=0, RXONLY=1), the
master starts the receive sequence as soon as the SPI is enabled. Then the clock signal is
provided by the master and it does not stop until either the SPI or the receive-only mode is
disabled by the master. The master receives data frames continuously up to this moment
------------------------------------------------------------------------------------------
Setting up GPIO for the SPI (Big gotcha)
------------------------------------------------------------------------------------------
I wanted to use PA2 and PA3 for CS# and WP, turns out these GPIO's are connected to USART2 rx and tx,

The USART2 interface available on PA2 and PA3 of the STM32 microcontroller can be
connected to ST-LINK MCU, ST morpho connector, or to ARDUINO® connector. The choice
can be changed by setting the related solder bridges.
By default, the USART2 communication between the target STM32 and ST-LINK MCU is enabled.

If the communication between the target STM32 PA2 (D1) or PA3 (D0) and shield or extension
board is required, SB62 and SB63 must be ON, while SB13 and SB14 must be OFF
See BoardGOTCHA.jpg

I then changed GPIO ports from PA2-3 to use PA13-14-15 for SPI GPIO (CS# and WP and HOLD)
The MCU wouldn't connect to ST-LINK, holding the reset button forced it to connect.
The MCU won't halt when inserting break points.
Flashing another project(6_ButtonLED_EXTI) fixed the issue, so something is up with my SPI GPIO initialization...
I commented it all out and it the project works as expected.
While writing this it hit me that I am an idiot that should quit this ... Back in my very first project I found out PA13-PA14 was being used by SWD
From project 1_BlinkyLED2 :
 Some registers have a reset value - This is to ensure functionalities such as SWD/JTAG is always enabled.
* PA13 and PA14 is used for SWDIO SWDCLK, therefor specific reset values for PORTA in GPIOA->MODER and GPIOA->PUPDR and GPIA->SPEEDR etc.

NOTE TO SELF: Be careful with PORT A !!

Switching the GPIOs to PB13-14-15 works as expected.
See GPIOTest_CS_WP_HOLD.jpg

------------------------------------------------------------------------------------------
Setting up the SPI
------------------------------------------------------------------------------------------

Master or slave operation
• Full-duplex synchronous transfers on three lines
  - In full-duplex mode, both devices can transmit and receive simultaneously, which is what we are going to use.
  - At each clock pulse, data is shifted in and out. 

From STM datasheet
Full-duplex communication
By default, the SPI is configured for full-duplex communication. In this configuration, the
shift registers of the master and slave are linked using two unidirectional lines between the
MOSI and the MISO pins. During SPI communication, data is shifted synchronously on the
SCK clock edges provided by the master. The master transmits the data to be sent to the
slave via the MOSI line and receives data from the slave via the MISO line. When the data
frame transfer is complete (all the bits are shifted) the information between the master and
slave is exchanged

• Half-duplex synchronous transfer on two lines (with bidirectional data line)
  - In half-duplex mode, each device can both transmit and receive, but not at the same time. When one device is sending, the other can only receive, and    vice versa.

• Simplex synchronous transfers on two lines (with unidirectional data line)
  - In Simplex mode, the communication is unidirectional, as on a one-way street. Only one of the two devices on a link can transmit, the other can only receive.

• 8-bit to 16-bit transfer frame format selection
 - As most of our commands for the external flash only uses 1 byte, we will start out with 8 bit configuration.

the SPI clock will only start when we transmit something, so dont expect it on a scope unless transmitting.
And don't forget to pull the CS# low.


Master mode fault (MODF)
Mode fault occurs when the master device has its internal SS signal (SS pin in SS hardware
mode, or SSI bit in SS software mode) pulled low. [...]
If you don't intend to use the NSS pin (N there stands for Negated), set both SPI_CFG2.SSM and SPI_CR1.SSI.



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


------------------------------------------------------------------------------------------------------------------------------
Had some issues understanding the SPI hardware, in full duplex mode we shift in and out of the rx and tx shift registers simultaneously.
That means when I shove dummy bytes in via MOSI I will get the slaves out data shifted right back via the MISO.

Its imperative that we check the flags set by hardware, e.g TXE and RXNE these flags tells us when the data has been shifted out of the internal buffers.
And if we have incoming data.

The overflow flag will trigger if we overwrite data, i.e. we dont read the SPI->DR, so we will overstuff the TX buffer and lose MISO data if we dont pick it up.





Also I need to clean up this document.....

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


