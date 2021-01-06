/* Includes ------------------------------------------------------------------*/
#include "customgpio.h"


/* Private define ------------------------------------------------------------*/
#define RESET_VAL_PORTA_MODER 0xEBFFFCFF
#define RESET_VAL_PORTA_PUPDR 0x24000000
#define RESET_VAL_PORTA_SPEEDR 0x0C000000 
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * Brief   This function configures 
  *         PA5 - User LED (LD2)
  *         PA8 - MCO 
  *                
  *         
  *         
  * 
  * Param   None
  * Retval  None
  */
void customGPIO_init(void)
{
    /* (1) Enable the peripheral clock of GPIOx */
    /* (2) Select Output mode GPIOx Pinx*/
    /* (3) Select Output type */
    /* (4) Select Pull-up / Pull-down  */
    /* (5) Select GPIO speed */
    /* (6) Select GPIO alternate function */
    /* (7) Select GPIO input */
    
  
    RCC->IOPENR |= RCC_IOPENR_IOPAEN; /* (1) */
    
 /**********************************************************************
     ** 				  LED2 Configurations PA5						  **
     **********************************************************************/
    GPIOA->MODER |= GPIO_MODER_MODE5_0; /* (2) */
    GPIOA->MODER &= ~GPIO_MODER_MODE5_1; /* (2) */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;  /* (3) PUSH PULL */
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED15_0;   /* (5) MEDIUM SPEED */
    
    /**********************************************************************
     ** 				  MCO Configurations PA8							  **
     **********************************************************************/
    GPIOA->MODER |= GPIO_MODER_MODE8_1; /* (6) */
    GPIOA->MODER &= ~GPIO_MODER_MODE8_0; /* (6) */
    GPIOA->PUPDR |= 1UL << 16;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED8_1; /* (5) */
    /* MCO Prescaler DIV8 */
    RCC->CFGR |= 1UL << 28 | 1UL << 29;
    RCC->CFGR |= RCC_CFGR_MCO_HSI;
 
}

/** TogglePort A Pin 5 (LD2) every second **/
void toggleLED2(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD5;
    
}
 