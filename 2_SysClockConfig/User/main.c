/**
  ******************************************************************************
  * @file    main.c
  * @author  -
  * @brief   This file implements the following:
  *          - Baremetal implementation using the CMSIS library for Cortex-M0+ 
  *          that switches the SYS_CLK from MSI to HSI and implements a delay function based on the 
  *          SysTick hardware - Nucleo-L053R8 STM32 development board.
  *          
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l053xx.h"
#include "customgpio.h"

/* Private define ------------------------------------------------------------*/
#define HSI_TIMEOUT_VALUE              ((uint32_t)100)  /* 100 ms */
#define REGULATOR_TIMEOUT_VALUE        ((uint32_t)500)  /* 500 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE      ((uint32_t)5000) /* 5 s    */

/* Delay values */
#define SEC_DELAY ((uint32_t)500)

/* Error codes */
#define ERROR_HSI_TIMEOUT 1
#define ERROR_REG_TIMEOUT 2
#define ERROR_CLKSWITCH_TIMEOUT 3

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t Tick;
uint8_t error = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
 
 
 /* Configure HSI 16 MHz */
 
 SystemClock_Config();
 SystemCoreClockUpdate();
 SysTick_Config(SystemCoreClock/1000);
 customGPIO_init();
 
  while (1)
  {
     /* TogglePort A Pin 5 (LD2) every second to check we set the clock right :) */
    GPIOA->ODR ^= (1 << 5);
    systick_delayms(SEC_DELAY); 
  }
}



/**
  * Brief   This function configures the system clock @ 16 MHz and voltage scale 1
  *         assuming the registers have their reset value before the call.
  *         POWER SCALE   = RANGE 1
  *         SYSTEM CLOCK  = HSI 16MHz        
  *         FLASH LATENCY = 1WS
  *         SYS_CLK is output for test purposes to MCO (PA8) with DIV4 (16MHz/2 = 2 MHz)  
  *  ****** IMPORTANT to call SystemCoreClockUpdate() after configuring the SYS_CLK ******
  * Param   None
  * Retval  None
  */

void SystemClock_Config(void)
{
  /* (1) Enable the clock for power settings on the APB1 bus  */
  /* (2) Select voltage scale  (VDD 1.71 - 3.6V)  VCORE 1.8V range 1 (see attachment)
  During voltage scaling configuration, the system clock is stopped until the regulator is
  stabilized (VOSF=0) - Poll VOSF bit of in PWR_CSR register. Wait until it is reset to 0 */
  /* (3) Configure FLASH Prefetch */ 
  /* (4) Enable HSI in RCC->CR */
  /* (5) Wait for HSI ready flag */
  /* (6) Select and check the HSI as SYS_CLK  */
  /* (7) Configure the MCO to DIV8 and to output the SYS_CLK (16MHz/8 = 2MHz)*/
  static uint32_t tickstart;
  
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */
     
  PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0; /* (2) */
  tickstart = Tick; // Intialize start time (Tick is incremented by SysTick_Handler every 1ms)
  while(PWR->CSR &= 1UL << 4) // Poll VOSF bit
  {
    if ((Tick - tickstart ) > REGULATOR_TIMEOUT_VALUE) // Report error after 500ms
    {
      error = ERROR_REG_TIMEOUT;
      return;
    }      
  }
  
 
  FLASH->ACR |= 0x1 | 1UL << 2; /* (3) */
  
  
  RCC->CR |= RCC_CR_HSION; /* (4) */
 
  tickstart = Tick;
  while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY)  /* (5) */
  {
    if ((Tick - tickstart ) > HSI_TIMEOUT_VALUE) // Report error after 100ms
    {
      error = ERROR_HSI_TIMEOUT;
      return;
    }      
  }
           
  
 RCC->CFGR |= 0x1; /* (6) */
 tickstart = Tick;
 while(!(RCC->CFGR & 1UL << 2) && (RCC->CFGR & 1UL << 3)) /* (6) */
 {
    if ((Tick - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      error = ERROR_CLKSWITCH_TIMEOUT; // Report error after 5s
      return;
    }      
  }
 
  /* MCO Prescaler DIV8 */
    RCC->CFGR |= 1UL << 28 | 1UL << 29;
    RCC->CFGR |= RCC_CFGR_MCO_HSI;
   
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