/* Includes ------------------------------------------------------------------*/
#include "customclock.h"
#include "system_stm32l0xx.h" // For SysTick_Handler

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * Brief   This function configures the system clock @ 24 MHz and voltage scale 1
  *         assuming the registers have their reset value before the call.
  *         POWER SCALE   = RANGE 1
  *         SYSTEM CLOCK  = PLL(HSI16) 24MHz
  *         PERIPHERAL CLOCKS = AHB_PRES = 2 -> APB1 APB2 = 12 MHz       
  *         FLASH LATENCY = 1WS
  *         SYS_CLK is output for test purposes to MCO (PA8) with DIV2  
  *  ****** IMPORTANT to call SystemCoreClockUpdate() after configuring the SYS_CLK ******
  * Param   None
  * Retval  None
  */
void Custom_clk_config(void)
{
  /* (1) Enable the clock for power settings on the APB1 bus  */
  /* (2) Select voltage scale  (VDD 1.71 - 3.6V)  VCORE 1.8V range 1 */
  /* (3) Configure FLASH Prefetch */ 
  /* (4) Enable HSI */
  /* (5) Wait for HSI ready flag */
  /* (6) Configure PLL, AHB, ABP1, ABP2 and MCO*/
  /* (7)  Enable PLL */
  /* (8) Wait for PLL ready flag */
  /* (9) Select PLL as system clock */
  /* (10) Wait for clock switched on PLL */ 
  /* (11) SYSCLK out on MCO for testing purposes (Remove comment to enable). 
    Remember to configure PA8 to alternate function */ 
  
/* 
    TODO:    (+) Enable the clock for the peripheral(s) to be used
             (+) Configure the clock source(s) for peripherals whose clocks are not
             derived from the System clock (ADC, RTC/LCD, RNG and IWDG)
*/
  // static uint32_t tickstart;
  
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */
     
  PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0; /* (2) */
  //tickstart = Tick; // Intialize start time (Tick is incremented by SysTick_Handler every 1ms).
  while(PWR->CSR &= PWR_CSR_VOSF) // Poll VOSF bit to see if the regulator is stabil.
  {
	  /*
    if ((Tick - tickstart ) > REGULATOR_TIMEOUT_VALUE) // Report error after 500ms.
    {
      error = ERROR_REG_TIMEOUT;
      return;
    }   */   
  }
  
  FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTEN; /* (3) */
  
  RCC->CR |= RCC_CR_HSION; /* (4) */
 
  while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY)  /* (5) */
  {
     
  }
  
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL3 | RCC_CFGR_PLLDIV2
  | RCC_CFGR_MCOPRE_DIV2 | RCC_CFGR_HPRE_DIV2 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1; /* (5) */
 
  RCC->CR |= RCC_CR_PLLON; /* (6) */
  
  while ((RCC->CR & RCC_CR_PLLRDY)  == 0) /* (0) */
  {

  }
 
  RCC->CFGR |= RCC_CFGR_SW_PLL; /* (9) */
  
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL)  == 0) /* (10) */
  {
    
  }
  
  // For testing purposes 
  RCC->CFGR |= RCC_CFGR_MCOSEL_PLL; /* (11) */
   
}

/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */