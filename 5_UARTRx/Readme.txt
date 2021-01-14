Lessons learned - 
Using an internal oscilator may not be the best choice, as they are inaccurate, this could easily result in UART frame errors.
The data sheet provides tolerance of the USART receiver to clock deviation at page 773
The HSI in our project is not calibrated, but it still works as intended.

------------ USART TX ---------------
Baud rate = number of bits / second so 115200 is a bit every 1*10^6/115200 = 8.68 uS
As long as we stay below our clock speed we will be fine.
Say we want to run at 115200 baud, the minimum clock we need is 120KHz clock at 8.33 uS per clock cycle.
If we want a baud of 9600 we need to be faster than 104 uS so even a 32KHz clock would be enough at 31.25 uS per clock cycle.


------------ USART RX ---------------
To determine max baud rate, note the oversampling value e.g. 16 .. check that CLK_MCU time > (baudrate time * 16).
Remember to account for tolerances.
If we send a bit every 8.68 uS (baud 115200) and we have to sample that bit 16 times, that means our CLK needs to be faster than 0.54 uS.
As we run our APB2 clock at 12MHz that means we can sample every 83.33 nS so our check is OK.

Say we have a 1MHz clock and a baudrate of 250.000 at 16 oversampling our clock cycle is every 1 uS and we send a bit every 250 nS this will collide, even with a 8 oversampling (500 nS). So we need at least a 2MHz clock and an oversampling of 8 to break even, but this will still result in failure because of the error margin.

The general rule of thumb is 5% error so in our example we need to add 83.33 * 1.05 as our minimum time, clocks arent perfect !


The baud rate does not depend on how many X you oversample. That is just a noise reduction strategy. It does help with speed mismatch but devices with quartz oscillators are not going to have much speed mismatch anyway

https://electronics.stackexchange.com/questions/494292/how-do-you-determine-which-baud-rate-to-choose-its-relation-with-mcu-clock-and

/**
     * set baud rate to 9600
     * 8MHz/(16*9600) = 52.08
     * DIV_Fraction = 16*0.08 = 1.28 -> 1 = 0x1
     * DIC_Mantissa = mantissa(52.08) = 52 = 0x34
     * BRR = 0x341
     */
    USART1->BRR = 0x341;


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


