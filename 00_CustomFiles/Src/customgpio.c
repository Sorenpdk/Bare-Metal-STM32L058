/* Includes ------------------------------------------------------------------*/
#include "customgpio.h"

/** TODO - Make a struct ? 
initialize the struct
send that struct to a handler that makes the assignments to the various registers.
which is better??
**/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Pins used overview

PA0
PA4
PA5
PA8
PA9
PA10
PB3
PB4
PB5
PB13
PB14
PC0
PC1
PC2
PC3
PC13
PC14
PC15
*/

/**
  * Brief   This function configures the STM32L058 GPIO
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
	GPIOA->MODER |= GPIO_MODER_MODE5_0; 			/* (2) GENERAL OUTPUT */
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1; 			/* (2) */
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;  			/* (3) PUSH PULL */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED15_0;     /* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		  MCO Configurations PA8			 **
	**********************************************************************/
	GPIOA->MODER |= GPIO_MODER_MODE8_1; 			/* (6) ALTERNATE */ 
	GPIOA->MODER &= ~GPIO_MODER_MODE8_0; 			/* (6) */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED8_1;		/* (5) HIGH SPEED */

	/* MCO Prescaler DIV8 */
	RCC->CFGR |= 1UL << 28 | 1UL << 29;
	RCC->CFGR |= RCC_CFGR_MCOSEL_PLL;

	/**********************************************************************
	** 		TIMER2_CH1 PWM Configurations PA0		 **
	**********************************************************************/
	GPIOA->MODER |= GPIO_MODER_MODE0_1; 			/* (6) ALTERNATE */
	GPIOA->MODER &=  ~GPIO_MODER_MODE0_0; 			/* (6) */
	GPIOA->AFR[0] |= 1UL << 1; 						/* (6) ALTERNATE */  

	/**********************************************************************
	** 		USART1_TX Configurations PA9		 **
	**********************************************************************/
	GPIOA->MODER |= GPIO_MODER_MODE9_1; 			/* (6) ALTERNATE */
	GPIOA->MODER &=  ~GPIO_MODER_MODE9_0;  			/* (6) */
	GPIOA->AFR[1] |= 4UL << 4;                		/* (6) ALTERNATE */

	/**********************************************************************
	** 		USART1_RX Configurations PA10		 **
	**********************************************************************/
	GPIOA->MODER |= GPIO_MODER_MODE10_1; 			/* (6) ALTERNATE */
	GPIOA->MODER &=  ~GPIO_MODER_MODE10_0;  		/* (6) */
	GPIOA->AFR[1] |= 4UL << 8;                		/* (6) ALTERNATE */

	/**********************************************************************
	** 		ADC1_CH.. the temperature example doesnt use a pin	 **
	**********************************************************************/


	/**********************************************************************
	** 		SPI1 MISO Configurations PB4		 **
	**********************************************************************/
	GPIOB->MODER |= GPIO_MODER_MODE4_1; 			/* (6) ALTERNATE */
	GPIOB->MODER &= ~GPIO_MODER_MODE4_0; 			/* (6) */
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_4; 			/* (3) PUSH PULL */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_1;   	/* (5) VERY HIGH SPEED */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_0;		/* (5) VERY HIGH SPEED */
	//GPIOB->AFR[0] |= 0UL << 20;          			/* (6) ALTERNATE */

	/**********************************************************************
	** 		SPI1 MOSI Configurations PB5		 **
	**********************************************************************/
	GPIOB->MODER |= GPIO_MODER_MODE5_1; 			/* (6) ALTERNATE */
	GPIOB->MODER &= ~GPIO_MODER_MODE5_0; 			/* (6) ALTERNATE */
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_5;  			/* (3) PUSH PULL */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_1;   	/* (5) VERY HIGH SPEED */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_0;		/* (5) VERY HIGH SPEED */
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD5_1; 			/* (4) PULL DOWN */
	GPIOB->AFR[0] |= 0UL << 20;          			/* (6) ALTERNATE */

	/**********************************************************************
	** 		SPI1 SCLK Configurations PB3		 **
	**********************************************************************/
	GPIOB->MODER |= GPIO_MODER_MODE3_1;  			/* (6) ALTERNATE */
	GPIOB->MODER &= ~GPIO_MODER_MODE3_0; 			/* (6) ALTERNATE */
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;  			/* (3) PUSH PULL */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_1;   	/* (5) VERY HIGH SPEED */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_0;   	/* (5) VERY HIGH SPEED */
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD3_0; 			/* (4) PULL UP */
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3_1);			/* (4) PULL UP */
	// GPIOB->AFR[0]                				/* (6) ALTERNATE */

	/**********************************************************************
	** 		SPI1 WP Configurations 	PB13	 	**
	**********************************************************************/
	GPIOB->MODER |= GPIO_MODER_MODE13_0;         	/* (2) GENERAL OUTPUT */
	GPIOB->MODER &= ~GPIO_MODER_MODE13_1;        	/* (2) GENERAL OUTPUT */ 
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_13;         	/* (3) PUSH PULL */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_1;  	/* (5) VERY HIGH SPEED */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_0;  	/* (5) VERY HIGH SPEED */
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD13_0;			/* (4) PULL UP */
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13_1);			/* (4) PULL UP */

	/**********************************************************************
	** 		SPI1 HOLD Configurations PB14		**
	**********************************************************************/
	GPIOB->MODER |= GPIO_MODER_MODE14_0; 			/* (2) GENERAL OUTPUT */
	GPIOB->MODER &= ~GPIO_MODER_MODE14_1; 			/* (2) GENERAL OUTPUT */ 
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_14;  			/* (3) PUSH PULL */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_1;  	/* (5) VERY HIGH SPEED */
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_0;  	/* (5) VERY HIGH SPEED */
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD14_0;			/* (4) PULL UP */
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD14_1);      	/* (4) PULL UP */

	/**********************************************************************
	** 		SPI1 CS Configurations PA4 		 	**
	**********************************************************************/
	// from slave perspective - When the device is not in operation and CS# is high, it is put in standby mode.
	GPIOA->MODER |= GPIO_MODER_MODE4_0; 			/* (2) GENERAL OUTPUT */
	GPIOA->MODER &= ~GPIO_MODER_MODE4_1; 			/* (2) GENERAL OUTPUT */ 
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;  			/* (3) PUSH PULL */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_1; 		/* (5) VERY HIGH SPEED */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED4_0;   	/* (5) VERY HIGH SPEED */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD4_0;				/* (4) PULL UP */
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4_1);			/* (4) PULL UP */

	/**********************************************************************
	** 		LCD DB4 Configurations 	PC0 - This is regular GPIO	 **
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE0_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE0_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_0;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED0_0;    	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		LCD DB5 Configurations 	PC1	 		**
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE1_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE1_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_1;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED1_0;    	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		LCD DB6 Configurations 	PC2	 		**
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE2_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE2_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_2;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED2_0;    	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		LCD DB7 Configurations 	PC3	 		**
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE3_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE3_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_3;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_0;    	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		LCD EN Configurations 	PC13	 	**
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE13_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE13_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_0;   	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 		LCD RW Configurations 	PC14	 	**
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE14_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE14_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_14;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_0;   	/* (5) MEDIUM SPEED */

	/**********************************************************************
	** 	LCD RS PC15 Register Select: H:Data Input L:Instruction Input   **
	**********************************************************************/
	GPIOC->MODER |= GPIO_MODER_MODE15_0; 			/* (2) GENERAL OUTPUT */
	GPIOC->MODER &= ~GPIO_MODER_MODE15_1; 			/* (2) GENERAL OUTPUT */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_15;  			/* (3) PUSH PULL */
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEED15_0;   	/* (5) MEDIUM SPEED */
	
} 

// Good candidate for inline (might change speed of I/O)
void toggleLED2(void)
{
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
