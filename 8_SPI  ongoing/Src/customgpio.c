/* Includes ------------------------------------------------------------------*/
#include "customgpio.h"


/* Private define ------------------------------------------------------------*/

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
    
  
    RCC->IOPENR |= RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN; /* (1) */
    
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
  GPIOA->MODER |= GPIO_MODER_MODE8_1; /* (6) ALTERNATE */ 
  GPIOA->MODER &= ~GPIO_MODER_MODE8_0; /* (6) */
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
   ** 		SPI MISO Configurations PB4		 **
   **********************************************************************/
    
  // input mode
  // push pull
  // pull down (not needed but good practice, the input is floating and operated by the slave)
  // Check what happens with MISO when you drive the chip select high / low
 
  
  /**********************************************************************
   ** 		SPI MOSI Configurations PB5		 **
   **********************************************************************/
   GPIOB->MODER |= GPIO_MODER_MODE5_1; /* (2) Alternate function */
   GPIOB->MODER &= ~GPIO_MODER_MODE5_0; /* (2) Clear bit */ 
   GPIOB->OTYPER &= ~GPIO_OTYPER_OT_5;  /* (3) PUSH PULL */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_1;   /* (5) Very High */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_0;
   GPIOB->PUPDR |= GPIO_PUPDR_PUPD5_1; // pull down
   GPIOB->AFR[0] |= 0UL << 20;          // AF0 is reset value
  
  /**********************************************************************
   ** 		SPI SCLK Configurations PB3		 **
   **********************************************************************/
 
   GPIOB->MODER |= GPIO_MODER_MODE3_1;  /* (2) Alternate function */
   GPIOB->MODER &= ~GPIO_MODER_MODE3_0; /* (2) Clear bit */ 
   GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;  /* (3) PUSH PULL */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_1;   /* (5) Very High */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_0;   
   
   GPIOB->PUPDR |= GPIO_PUPDR_PUPD13_0; // pull up
   // GPIOB->AFR[0]                AF0 is reset value
  /**********************************************************************
   ** 		SPI WP Configurations 	PB13	 **
   **********************************************************************/
  
   GPIOB->MODER |= GPIO_MODER_MODE13_0;                 /* (2) GENERAL OUTPUT  */
   GPIOB->MODER &= ~GPIO_MODER_MODE13_1;                /* (2) GENERAL OUTPUT */ 
   GPIOB->OTYPER &= ~GPIO_OTYPER_OT_13;                 /* (3) PUSH PULL */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_1;          /* (5) Very High */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_0; 
   GPIOB->PUPDR |= GPIO_PUPDR_PUPD13_0;
   GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13_1);
  
  /**********************************************************************
   ** 		SPI HOLD Configurations PB14		 **
   **********************************************************************/
   GPIOB->MODER |= GPIO_MODER_MODE14_0; /* (2) GENERAL OUTPUT  */
   GPIOB->MODER &= ~GPIO_MODER_MODE14_1; /* (2) GENERAL OUTPUT */ 
   GPIOB->OTYPER &= ~GPIO_OTYPER_OT_14;  /* (3) PUSH PULL */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_1; /* (5) Very High */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_0;   
   GPIOB->PUPDR |= GPIO_PUPDR_PUPD14_0;
   GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD14_1);
  /**********************************************************************
   ** 		SPI CS Configurations PA4 		 **
   **********************************************************************/
   // from slave perspective - When the device is not in operation and CS# is high, it is put in standby mode.
   GPIOA->MODER |= GPIO_MODER_MODE4_0; /* (2) GENERAL OUTPUT  */
   GPIOA->MODER &= ~GPIO_MODER_MODE4_1; /* (2) GENERAL OUTPUT */ 
   GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;  /* (3) PUSH PULL */
   GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_1; /* (5) Very High */
   GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_0;   
   GPIOA->PUPDR |= GPIO_PUPDR_PUPD4_0;
   GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4_1);
   
}

/** TogglePort A Pin 5 (LD2) every second **/
void toggleLED2(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD5;
    
}
 