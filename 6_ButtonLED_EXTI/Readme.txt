*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

--------- Clock / registers ---------


--------- CM0+ / STM32 --------- 


--------- Protocol ---------


--------- Software / Basic operations ---------
Turns out the push button B1 on PC13 is hardware debounced, I was suprised to see it working flawlessly when lighting up the LED directly from
the interrupt handler for a GPIO - which is normally super bad practice, as debounces/noisy inputs aren't filtered.

At some point i will create an example where we software debounce, a Counting Algorithm.
using a timer interrupt/polling the gpio of B1 and implementing some form of statistical evaluation of
whether or not the button is pressed or released.








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


