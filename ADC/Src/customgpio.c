/* Includes ------------------------------------------------------------------*/
#include "customgpio.h"
#include "customdelay.h"

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
	
        /* (1) */
	RCC->IOPENR |= RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN;
        
        
        // PORT A configs
        GPIOA->MODER = (GPIOA->MODER & RESET_VAL_PORTA_MODER) | (1<<10); /* (2) GEN. PURPOSE OUTPUT */
        GPIOA->MODER = (GPIOA->MODER & RESET_VAL_PORTA_MODER) & ~(1<<11); /* (2) */
        GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;  /* (3) PUSH PULL */
        GPIOA->PUPDR = (GPIOA->PUPDR & RESET_VAL_PORTA_PUPDR) | GPIO_PUPDR_PUPD5_0;  /* (4) PULL UP */
        GPIOA->OSPEEDR = (GPIOA->OSPEEDR & RESET_VAL_PORTA_SPEEDR) | GPIO_OSPEEDER_OSPEED15_0;   /* (5) MEDIUM SPEED */
        
        // PORT B configs
 
}

/** TogglePort A Pin 5 (LD2) every second **/
void toggleLED2(void)
{
    GPIOA->ODR ^= (1 << 5);
    customDelay(SEC_DELAY/2); 
}
 