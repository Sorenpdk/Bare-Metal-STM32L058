/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *          that switches the SYS_CLK from MSI to 24MHz PLL(HSI16) and implements 
  *          a delay function based on the  SysTick hardware - 
  *          Nucleo-L053R8 STM32 development board.
  *          
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "customclock.h"
#include "stm32l0xx.h"

/* Private define ------------------------------------------------------------*/


/* Delay values */
#define SEC_DELAY ((uint32_t)500)

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t Tick;

/* Private function prototypes -----------------------------------------------*/
void ConfigureGPIO(void);

void systick_delayms(int ms);

/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
  
/* The MSI clock is always used as system clock after restart from Reset 2.097 MHz (default value)*/
 SysTick_Config(SystemCoreClock/1000); /* 1ms config */
 Custom_clk_config();
 SystemCoreClockUpdate();
 ConfigureGPIO();
 SysTick_Config(12000); // 12 MHz

 
  while (1)
  {
     /* TogglePort A Pin 5 (LD2) every second to check we set the clock right :) */
    GPIOA->ODR ^= (1 << 5);
    systick_delayms(SEC_DELAY); 
  }
}

__INLINE void ConfigureGPIO(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (3) Select output mode (01) on GPIOA pin 5 */
  /* (4) Select alternate function mode on GPIOA pin 8 */
  /* (5) Select pull down on GPIOA pin 8*/
  /* (6) Select high speed for GPIOA pin 8 (max 50MHz) - OSPEEDR8 = 11 */
  
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN; /* (1) */ 
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE5)) | (GPIO_MODER_MODE5_0); /* (2) */   
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE8)) | (GPIO_MODER_MODE8_1); /* (4) */
 // GPIOA->PUPDR = (GPIOA->PUPDR & ~(1UL << 16)) | (1UL << 17);
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED8_1; /* (6) */
  
  /* MCO is AFR0 on PA8 and Alternate function 0 is selected by default 
     so no real need to modify GPIOA_AFR - MCO can be used to transfer MCU clock to a peripheral */
}


/******************************************************************************/
/*            Cortex-M0 Plus Processor Exceptions Handlers                    */
/******************************************************************************/

/**
  * Brief   This function implements the SysTick handler,
  *         which increments the variable Tick every millisecond.
  *         
  *         
  * Param   None
  * Retval  None
  */
void SysTick_Handler(void)
{
  Tick++;
  
}

void systick_delayms(int ms)
{
  Tick = 0;
  while(Tick < ms);
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */