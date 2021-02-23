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
#include "stm32l053xx.h"
#include "stdio.h"
/* Standard includes. */
#include "string.h"
#include "customgpio.h"
#include "customclock.h"
#include "customuart.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

void myTask1(void *p)
{
  
  while(1)
  {
   toggleLED2();
    
   vTaskDelay(500);
  }
  
}

void myTask2(void *p)
{
  uint8_t counter = 0;
   
  while(1)
  {
    counter++;

    char array[10] = {0};
    char *p = array;
    sprintf(array, "%d",  counter);
    
    UART_Send_String((uint8_t*)p,sizeof(p)); // usart sends ascii in terminal ... yikes
    UART_Send_Char(0x0D); // CR
    UART_Send_Char(0x0A); // LF
    
    vTaskDelay(250);
  }
  
}


int main(void)
{
  customClock_init();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  customGPIO_init();
  customUSART1_Init();
 
  xTaskCreate(myTask1,"Blinky", 20, (void*)0, tskIDLE_PRIORITY, &myTask1Handle);
  xTaskCreate(myTask2,"USART", 100, (void*)0, tskIDLE_PRIORITY+1, &myTask2Handle);
  
  vTaskStartScheduler();
  
  while (1)
  {
   
  }
}

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}




/* https://www.st.com/resource/en/reference_manual/dm00095744-ultralowpower-stm32l0x3-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

