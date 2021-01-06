*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

--------- Clock / registers ---------
PLL - Phase locked loop.
- Jitter and noise reduction
- Clock multiplier
- As system clock frequencies reach 100 MHz and beyond, maintaining control over clock becomes very important.
- TI application note https://www.ti.com/lit/an/snoa351/snoa351.pdf
- https://electronics.stackexchange.com/questions/514204/what-is-the-purpose-of-pll-in-a-general-microcontroller
Cache prefetching is a technique used by computer processors to boost execution performance by fetching instructions or data from their original storage in slower memory to a faster local memory before it is actually needed (hence the term 'prefetch')
- https://www.st.com/resource/en/application_note/dm00237631-stm32-power-mode-examples-stmicroelectronics.pdf

Latency: Flash memory is slower than the CPU.
So, if you need to run the CPU at full speed (which is optional, because lower speed saves power), then the number of wait states need to be programmed. IIRC, the CPU can run without any wait states at 24MHz or below. Running at those lower speeds will save significant power in a low power application, which still running with good performance.

"on-the-fly": the CPU can adjust the clock speed while it is running.
Hence, to get the best throughput vs power, it might run at a low-clock speed to save power, while still doing work, then when needed, crank up the clock, when flash will need more wait cycles to work reliably.

Cache: the prefetch buffer is a simple type of cache.
The internal flash memory is 64 bits wide. The pre-fetch buffer is the same width, so it can hold upto 4 instructions in each of its two blocks. AFAIK it is least recently used, and hence just cycles. I imagine having two blocks simplifies the circuitry, and avoids contention between program load and instruction decode.
****STM32l0 series doesn't have a 64 bit wide bus!*****
https://electronics.stackexchange.com/questions/125136/whats-the-concept-of-these-three-options-for-configuring-the-memory-of-stm32





Power settings for the clock.
MCO - Clock output to other devices, used for clock sync.
--------- CM0+ / STM32 --------- 
If debugger is set to simulator :) You will see 0xCDCDCDCD in all registers
#define 	STACK_FILL_VALUE   0xCDCDCDCD
 	Define the magic value that is interpreted by IAR C-SPY's Stack View
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


