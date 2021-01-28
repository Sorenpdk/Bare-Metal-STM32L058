*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

--------- Clock / registers ---------


--------- CM0+ / STM32 --------- 
Each MCU is individually factory-calibrated by ST. The temperature sensor factory calibration data are
stored by ST in the system memory area, accessible in read-only mode. 

to access the value we provide a pointer to the specific memory adress 
/* Temperature sensor calibration value address */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))

when used, we dereference the pointer as such value = *TEMP130_CAL_ADDR
this can then be typecasted to whatever, as the value is only around 600 in our case a 16 bit integer is suffcient.
***** The calibration by ST is performed at 3V ! ******
The current VDDA voltage is 3.3V so we need to scale accordingly, at least for our temperature measurements.

--------- Protocol ---------


--------- Software / Basic operations ---------

The ADC is quite complicated! I need to make a "white paper" to understand it all.







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


