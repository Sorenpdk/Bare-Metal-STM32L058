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
#include "stm32l0xx.h"
#include "customclock.h"
#include "customgpio.h"
#include "customdelay.h"
/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
 customClock_init();
 SystemCoreClockUpdate();
 
 customDelay_init();
 customGPIO_init();


  while (1)
  {
    toggleLED2();
  
  }
}


/** 
Software must write to the ADCAL and ADEN bits in the ADC_CR register if the ADC is
disabled (ADEN must be 0)

Software must only write to the ADSTART and ADDIS bits in the ADC_CR register only if
the ADC is enabled and there is no pending request to disable the ADC (ADEN = 1 and
ADDIS = 0). 

Software must only write to the ADSTP bit in the ADC_CR register if the ADC is enabled
(and possibly converting) and there is no pending request to disable the ADC (ADSTART =
1 and ADDIS = 0).

The total conversion time is calculated as follows:
tCONV = Sampling time + 12.5 x ADC clock cycles
Example:
With ADC_CLK = 16 MHz and a sampling time of 1.5 ADC clock cycles:
tCONV = 1.5 + 12.5 = 14 ADC clock cycles = 0.875 µs

ADC voltage regulator (ADVREGEN)
The ADC has a specific internal voltage regulator which must be enabled and stable before
using the ADC. The ADC voltage regulator stabilization time is entirely managed by the hardware and
software does not need to care about it.

Follow this procedure to enable the ADC:
1. Clear the ADRDY bit in ADC_ISR register by programming this bit to 1.
2. Set ADEN=1 in the ADC_CR register.
3. Wait until ADRDY=1 in the ADC_ISR register (ADRDY is set after the ADC startup
time). This can be handled by interrupt if the interrupt is enabled by setting the
ADRDYIE bit in the ADC_IER register.

The ADC clock can be derived from the APB clock of the ADC bus interface,
divided by a programmable factor (2 or 4) according to bits CKMODE[1:0].
To select this scheme, bits CKMODE[1:0] of the ADC_CFGR2 register must be
different from “00”. 
CKMODE[1:0] = 11 for

• 16 analog inputs from GPIO pins (ADC_INx)


Programmable sampling time (SMP)
This programmable sampling time is common to all channels. If required by the application,
the software can change and adapt this sampling time between each conversions.
The total conversion time is calculated as follows:
tCONV = Sampling time + 12.5 x ADC clock cycles
Example:
With ADC_CLK = 16 MHz and a sampling time of 1.5 ADC clock cycles:
tCONV = 1.5 + 12.5 = 14 ADC clock cycles = 0.875 µs
The ADC indicates the end of the sampling phase by setting the EOSMP flag.
*/

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

