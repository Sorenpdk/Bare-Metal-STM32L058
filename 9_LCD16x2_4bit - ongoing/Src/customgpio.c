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
    
  
    RCC->IOPENR |= RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN | RCC_IOPENR_IOPCEN ; /* (1) */
    
   /********************************************************************
   ** 				  LED2 Configurations PA5	**
   **********************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODE5_0; /* (2) GENERAL OUTPUT */
  GPIOA->MODER &= ~GPIO_MODER_MODE5_1; /* (2) */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;  /* (3) PUSH PULL */
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED15_0;   /* (5) MEDIUM SPEED */
  
  /**********************************************************************
   ** 		  MCO Configurations PA8			 **
   **********************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODE8_1; /* (2) ALTERNATE */ 
  GPIOA->MODER &= ~GPIO_MODER_MODE8_0; /* (2) */
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED8_1; /* (5) */
  
  /* MCO Prescaler DIV8 */
  RCC->CFGR |= 1UL << 28 | 1UL << 29;
  RCC->CFGR |= RCC_CFGR_MCOSEL_PLL;
  /**********************************************************************
   ** 		TIMER2_CH1 PWM Configurations PA0		 **
   **********************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODE0_1; /* (6) ALTERNATE */
  GPIOA->MODER &=  ~GPIO_MODER_MODE0_0;  /* (6) */
  GPIOA->AFR[0] |= 1UL << 1; /* (6) */  
    
   /**********************************************************************
   ** 		USART1_TX Configurations PA9		 **
   **********************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODE9_1; /* (6) ALTERNATE */
  GPIOA->MODER &=  ~GPIO_MODER_MODE9_0;  /* (6) */
  
  GPIOA->AFR[1] |= 4UL << 4;                /* (3) */
  
  /**********************************************************************
   ** 		USART1_RX Configurations PA10		 **
   **********************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODE10_1; /* (6) ALTERNATE */
  GPIOA->MODER &=  ~GPIO_MODER_MODE10_0;  /* (6) */
  GPIOA->AFR[1] |= 4UL << 8;                /* (3) */
 
   /**********************************************************************
   ** 		LCD DB4 Configurations 	PC0	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE0_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE0_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_0;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED0_0;   /* (5) MEDIUM SPEED */
  /**********************************************************************
   ** 		LCD DB5 Configurations 	PC1	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE1_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE1_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_1;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED1_0;   /* (5) MEDIUM SPEED */
  /**********************************************************************
   ** 		LCD DB6 Configurations 	PC2	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE2_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE2_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_2;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED2_0;   /* (5) MEDIUM SPEED */
  /**********************************************************************
   ** 		LCD DB7 Configurations 	PC3	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE3_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE3_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_3;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_0;   /* (5) MEDIUM SPEED */
  /**********************************************************************
   ** 		LCD EN Configurations 	PC13	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE13_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE13_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_1;   /* (5) MEDIUM SPEED */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_0;   /* (5) MEDIUM SPEED */
  /**********************************************************************
   ** 		LCD RW Configurations 	PC6	 **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE6_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE6_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_6;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_0;   /* (5) MEDIUM SPEED */
   /**********************************************************************
   ** 	LCD RS PC10 Register Select: H:Data Input L:Instruction Input        **
   **********************************************************************/
  GPIOC->MODER |= GPIO_MODER_MODE10_0; /* (2) GENERAL OUTPUT */
  GPIOC->MODER &= ~GPIO_MODER_MODE10_1; /* (2) */
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_10;  /* (3) PUSH PULL */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED10_0;   /* (5) MEDIUM SPEED */
}

/** TogglePort A Pin 5 (LD2) every second **/
void toggleLED2(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD5;
    
}
 