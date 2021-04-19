/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "usart.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
osThreadId Sender_HPTHandle;
osThreadId Sender_LPTHandle;
osThreadId ReceiverHandle;
osMessageQId SimpleQHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartSender_HPT(void const * argument);
void StartSender_LPT(void const * argument);
void StartReceiver(void const * argument);

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
  /* definition and creation of SimpleQ */
  osMessageQDef(SimpleQ, 16, uint16_t);
  SimpleQHandle = osMessageCreate(osMessageQ(SimpleQ), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  if(SimpleQHandle == 0){
	 char*str = "Unable to create Queue\n\n";
	 HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
  }
  else{
	  char*str = "Queue Created\n\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
  }

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Sender_HPT */
  osThreadDef(Sender_HPT, StartSender_HPT, osPriorityHigh, 0, 128);
  Sender_HPTHandle = osThreadCreate(osThread(Sender_HPT), NULL);

  /* definition and creation of Sender_LPT */
  osThreadDef(Sender_LPT, StartSender_LPT, osPriorityNormal, 0, 128);
  Sender_LPTHandle = osThreadCreate(osThread(Sender_LPT), NULL);

  /* definition and creation of Receiver */
  osThreadDef(Receiver, StartReceiver, osPriorityLow, 0, 128);
  ReceiverHandle = osThreadCreate(osThread(Receiver), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartSender_HPT */
/**
  * @brief  Function implementing the Sender_HPT thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartSender_HPT */
void StartSender_HPT(void const * argument)
{
  /* USER CODE BEGIN StartSender_HPT */
	int i = 222;
	uint32_t TickDelay = pdMS_TO_TICKS(2000);
  /* Infinite loop */
  for(;;)
  {
	  char*str = "Entered SENDER HPT Task\n Sendig a number to the queue\n\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);

	  if (xQueueSend(SimpleQHandle, &i, portMAX_DELAY) == pdPASS){
		  char*str2 = "Successfully sent the number\n Leaving SENDER HPT\n\n";
		  HAL_UART_Transmit(&huart2, (uint8_t*)str2, strlen(str2), HAL_MAX_DELAY);
	  }
    osDelay(TickDelay);
  }
  /* USER CODE END StartSender_HPT */
}

/* USER CODE BEGIN Header_StartSender_LPT */
/**
* @brief Function implementing the Sender_LPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSender_LPT */
void StartSender_LPT(void const * argument)
{
  /* USER CODE BEGIN StartSender_LPT */
	int s = 111;
	uint32_t TickDelay = pdMS_TO_TICKS(1000);
  /* Infinite loop */
  for(;;)
  {
	  char*str = "Entered SENDER LPT Task\n Sendig a number to the queue\n\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);

	 xQueueSend(SimpleQHandle, &s, portMAX_DELAY);

	 char*str2 = "Successfully sent the number\n Leaving SENDER LPT\n\n";
	 HAL_UART_Transmit(&huart2, (uint8_t*)str2, strlen(str2), HAL_MAX_DELAY);

    osDelay(TickDelay);
  }
  /* USER CODE END StartSender_LPT */
}

/* USER CODE BEGIN Header_StartReceiver */
/**
* @brief Function implementing the Receiver thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReceiver */
void StartReceiver(void const * argument)
{
  /* USER CODE BEGIN StartReceiver */
	int received = 0;
	uint32_t TickDelay = pdMS_TO_TICKS(5000);
  /* Infinite loop */
  for(;;)
  {
	  char str[100];
	  strcpy (str, "Entered RECEIVER task\n About to receive a number from the queue\n");
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);

	  if(xQueueReceive(SimpleQHandle, &received, portMAX_DELAY) != pdTRUE){
		  HAL_UART_Transmit(&huart2, (uint8_t*)"Error in queue", 14, 1000);
	  }
	  else{
		  sprintf(str, "Successfully received the number %d\n Leaving Receiver\n\n", received);
		  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
	  }
    osDelay(TickDelay);
  }
  /* USER CODE END StartReceiver */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
