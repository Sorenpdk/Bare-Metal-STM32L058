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
#include "customuart.h"
#include "string.h"
#include "stdio.h"
/* Private define ------------------------------------------------------------*/
/* Temperature sensor calibration value address */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300)) // Device was calibrated at 3V
#define VDD_APPLI ((uint16_t) (330)) // actual VDDA is measured to 3.3V
   
/* Private variables ---------------------------------------------------------*/      
uint8_t temperature_C; //contains the computed temperature
char buffer[16];
/* Private function prototypes -----------------------------------------------*/
void SetClockForADC(void);
void CalibrateADC(void);
void ConfigureADC(void);
void EnableADC(void);
void DisableADC(void);
uint8_t ComputeTemperature(uint8_t measure);

/**
  * Brief   Main program.
  * Param   None
  * Retval  None
  */
int main(void)
{
 customClock_init();
 SystemCoreClockUpdate();
 SysTick_Config(SystemCoreClock/1000);
 
 customDelay_init();
 customGPIO_init();
 customUSART1_Init();
 SetClockForADC();
  ConfigureADC();
  CalibrateADC(); 
  EnableADC();
  
  ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
  while (1)
  {
     while ((ADC1->ISR & ADC_ISR_EOC) == 0); /* Wait end of conversion */
    temperature_C = ComputeTemperature((uint8_t)ADC1->DR);  
    sprintf(buffer, "%d", temperature_C);   
    UART_Send_String((uint8_t*)buffer, sizeof(buffer));
    
    customDelay(1000);
  }
  
}


/**
  * Brief   This function enables the clock in the RCC for the ADC
  *        and for the System configuration (mandatory to enable VREFINT)
  * Param   None
  * Retval  None
  */
__INLINE void SetClockForADC(void)
{
  /* (1) Enable the peripheral clock of the ADC */
  
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; /* (1) */
}


/**
  * Brief   This function performs a self-calibration of the ADC
  * Param   None
  * Retval  None
  */
__INLINE void  CalibrateADC(void)
{
  /* (1) Ensure that ADEN = 0 */
  /* (2) Clear ADEN */ 
  /* (3) Set ADCAL=1 */
  /* (4) Wait until EOCAL=1 */
  /* (5) Clear EOCAL */
  if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
  {
    ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);  /* (2) */  
  }
  ADC1->CR |= ADC_CR_ADCAL; /* (3) */
  while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) /* (4) */
  {
    /* For robust implementation, add here time-out management */
  }
  ADC1->ISR |= ADC_ISR_EOCAL; /* (5) */
}


/**
  * Brief   This function configure the ADC to convert the temperature sensor
  *         The conversion frequency is 16MHz
  *         Be careful to enable the VREFINT and the buffer of the sensor 
  *         in the system configuration.
  * Param   None
  * Retval  None
  */
__INLINE void ConfigureADC(void)
{
  /* (1) Select HSI16 by writing 00 in CKMODE (reset value) The software is allowed to write these bits only when the ADC is disabled (ADCAL=0,
      ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0). */ 
  /* (2) Select continuous mode */
  /* (3) Select CHSEL18 for temperature sensor */
  /* (4) Select a sampling mode of 111 */
  /* with 16MHz clock the total conversion time is
    1/16*10^6 = 6.25*10^-8 -> 6.25*10^-2 = 0.0625 uS between clock cycles
    tconv = sampling time + 12.5 x ADC clock cycles 
    this is not a multiply sign!
    tconv = ADC cycles * samplingstime ->
    tconv = 160.5 + 12.5 = 173 * 0.0625 = 10.81 uS 
    so our sampling frequency would be about 92.5 KHz */
  /* (5) Wake-up the Temperature sensor (only for VLCD, Temp sensor and VRefInt) */
  //ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE; /* (1) */   
  
    ADC1->CFGR1 |= ADC_CFGR1_CONT; /* (2) */
  
  ADC1->CHSELR = ADC_CHSELR_CHSEL18; /* (3) */
  ADC1->SMPR |= ADC_SMPR_SMP; /* (4) */
  ADC->CCR |= ADC_CCR_TSEN; /* (5) */
}


/**
  * Brief   This function enables the ADC
  * Param   None
  * Retval  None
  */
__INLINE void EnableADC(void)
{
  /* (1) Enable the ADC */
  /* (2) Wait until ADC ready if AUTOFF is not set */
  ADC1->CR |= ADC_CR_ADEN; /* (1) */
  if ((ADC1->CFGR1 &  ADC_CFGR1_AUTOFF) == 0)
  {
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (2) */
    {
      /* For robust implementation, add here time-out management */
    }
  }
}


/**
  * Brief   This function disables the ADC
  * Param   None
  * Retval  None
  */
__INLINE void DisableADC(void)
{
  /* (1) Ensure that no conversion on going */
  /* (2) Stop any ongoing conversion */
  /* (3) Wait until ADSTP is reset by hardware i.e. conversion is stopped */
  /* (4) Disable the ADC */
  /* (5) Wait until the ADC is fully disabled */
  if ((ADC1->CR & ADC_CR_ADSTART) != 0) /* (1) */
  {
    ADC1->CR |= ADC_CR_ADSTP; /* (2) */
  }
  while ((ADC1->CR & ADC_CR_ADSTP) != 0) /* (3) */
  {
     /* For robust implementation, add here time-out management */
  }
  ADC1->CR |= ADC_CR_ADDIS; /* (4) */
  while ((ADC1->CR & ADC_CR_ADEN) != 0) /* (5) */
  {
    /* For robust implementation, add here time-out management */
  }  
}

/**
  * Brief   This function computes the temperature from the temperature sensor measure
  *         The computation uses the following formula :
  *         Temp = (Vsense - V30)/Avg_Slope + 30
  *         Avg_Slope = (V130 - V30) / (130 - 30)
  *         V30 = Vsense @30°C (calibrated in factory @3V)
  *         V130 = Vsense @110°C (calibrated in factory @3V)
  *         VDD_APPLI/VDD_CALIB coefficient allows to adapt the measured value
  *         according to the application board power supply versus the 
  *         factory calibration power supply.
  * Param   measure is the a voltage measured from the temperature sensor (can have been filtered)
  * Retval  returns the computed temperature
  */
uint8_t ComputeTemperature(uint8_t measure)
{
  int32_t temperature;
  
  temperature = ((measure * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR ) ;	
  temperature = temperature * (int32_t)(130 - 30);                      
  temperature = temperature / (int32_t)(*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);                 
  temperature = temperature + 30;
  return(temperature);
}











/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

