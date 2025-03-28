/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led_RGB_task.h"
#include "chassis_task.h"
#include "oled_task.h"
#include "ps2_task.h"
#include "uart_transmit_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

osThreadId led_RGB_TaskHandle;
osThreadId chassisTaskHandle;
osThreadId oledTaskHandle;
osThreadId ps2TaskHandle;
osThreadId UART_TransmitTaskHandle;

osMessageQId uarxQueueHandle;
osMessageQId ps2QueueHandle;

UBaseType_t xFreeStackSpace1;
UBaseType_t xFreeStackSpace2;
UBaseType_t xFreeStackSpace3;
UBaseType_t xFreeStackSpace4;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId testHandle;
osMessageQId myQueue01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void test_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	osMessageQDef(uarxQueue, 64, UART_Data_t);
  uarxQueueHandle = osMessageCreate(osMessageQ(uarxQueue), NULL);
	
	osMessageQDef(ps2Queue,128, PS2Data_TypeDef);
  ps2QueueHandle = osMessageCreate(osMessageQ(ps2Queue), NULL);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of test */
  osThreadDef(test, test_task, osPriorityNormal, 0, 128);
  testHandle = osThreadCreate(osThread(test), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	osThreadDef(LED_RGB, led_RGB_task, osPriorityNormal, 0, 128);
	led_RGB_TaskHandle = osThreadCreate(osThread(LED_RGB), NULL);
	
	osThreadDef(OLED, oled_task, osPriorityNormal, 0, 128);
	oledTaskHandle = osThreadCreate(osThread(OLED), NULL);
	
	osThreadDef(Chassis, chassis_task, osPriorityAboveNormal, 0, 128);
	chassisTaskHandle = osThreadCreate(osThread(Chassis), NULL);
		
	osThreadDef(PS2, ps2_task, osPriorityHigh, 0, 128);
	ps2TaskHandle = osThreadCreate(osThread(PS2), NULL);
	
	osThreadDef(UART_Transmit, UART_TransmitTask, osPriorityNormal, 0, 128);
	UART_TransmitTaskHandle = osThreadCreate(osThread(UART_Transmit), NULL);
		
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_test_task */
/**
  * @brief  Function implementing the test thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_test_task */
__weak void test_task(void const * argument)
{
  /* USER CODE BEGIN test_task */
//	 xFreeStackSpace1 = uxTaskGetStackHighWaterMark(ps2TaskHandle);
//	xFreeStackSpace2 = uxTaskGetStackHighWaterMark(led_RGB_TaskHandle);
//	xFreeStackSpace3 = uxTaskGetStackHighWaterMark(chassisTaskHandle);
//	xFreeStackSpace4 = uxTaskGetStackHighWaterMark(oledTaskHandle);
  /* Infinite loop */
  for(;;)
  {
  }
  /* USER CODE END test_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
