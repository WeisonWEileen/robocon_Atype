/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

// 这里配置每个任务参数  现在的格式：const名字小写大写, 函数任务大写大写，�?�空间由CubeMX里面设置，一共是15k = 128*8*15
osThreadId_t chasisTaskHandle;
const osThreadAttr_t chasisTask_attributes = {
    .name = "chasisTask",
    .stack_size = 128 * 2,
    .priority = (osPriority_t)osPriorityAboveNormal1,
};

osThreadId_t errorDetectTaskHandle;
const osThreadAttr_t errorDetectTask_attributes = {
    .name = "errorDetectTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t)osPriorityAboveNormal1,
};

osThreadId_t sdioTaskHandle;
const osThreadAttr_t sdioTask_attributes = {
    .name = "sdrwTask",
    .stack_size = 128 * 8 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};

// 这是挂载sd卡的句柄
osThreadId_t mount_open_sdTaskHandle;
const osThreadAttr_t mount_openTask_attributes = {
    .name = "mount_open_sdTask",
    .stack_size = 128 * 8,
    .priority = (osPriority_t)osPriorityHigh,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
__weak void ChasisTask(void *argument);
__weak void ErrorDetectTask(void *argument);
__weak void SdioTask(void *argument);
__weak void Mount_open_sdTask(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  // mount_open_sdTaskHandle = osThreadNew(Mount_open_sdTask, NULL, &mount_openTask_attributes);
  chasisTaskHandle = osThreadNew(ChasisTask, NULL, &chasisTask_attributes);
  // errorDetectTaskHandle = osThreadNew(ErrorDetectTask, NULL, &errorDetectTask_attributes);
  sdioTaskHandle = osThreadNew(SdioTask, NULL, &sdioTask_attributes);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

__weak void ChasisTask(void *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

__weak void ErrorDetectTask(void *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

__weak void SdioTask(void *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

__weak void Mount_open_sdTask(void *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

/* USER CODE END Application */

