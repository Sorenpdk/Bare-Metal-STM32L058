*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

--------- Clock / registers ---------


--------- CM0+ / STM32 --------- 
STM32L053R8 is a "category 3" device, so it does not feature TIM3 and TIM7 !
Datasheet section 1.4 Product category definition
Our initial wish to perform downcounting with TIM3 cannot be done, we will switch to TIM6

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


