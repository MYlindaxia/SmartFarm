/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "../ASP/oled/oled.h"
#include "../ASP/aht20/aht20.h"
#include <stdio.h>
#include "../ASP/encoder/encoder.h"
#include "../ASP/light/light.h"
#include "../ASP/ble/ble.h"
#include "../utils/FormatValue.h"
#include "../ASP/buzzer/buzzer.h"
#include "../utils/soft_I2C.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

char luxText[32];
char temperatureText[32];
char humidityText[32];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
char msg[64];
/* USER CODE END Variables */
/* Definitions for BLETask */
osThreadId_t BLETaskHandle;
const osThreadAttr_t BLETask_attributes = {
  .name = "BLETask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLEDTask */
osThreadId_t OLEDTaskHandle;
const osThreadAttr_t OLEDTask_attributes = {
  .name = "OLEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorTask */
osThreadId_t SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
  .name = "SensorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for InputTask */
osThreadId_t InputTaskHandle;
const osThreadAttr_t InputTask_attributes = {
  .name = "InputTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BLEQueue */
osMessageQueueId_t BLEQueueHandle;
const osMessageQueueAttr_t BLEQueue_attributes = {
  .name = "BLEQueue"
};
/* Definitions for I2C_Bus_Mutex */
osMutexId_t I2C_Bus_MutexHandle;
const osMutexAttr_t I2C_Bus_Mutex_attributes = {
  .name = "I2C_Bus_Mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBLETask(void *argument);
void StartOLEDTask(void *argument);
void StartSensorTask(void *argument);
void StartInputTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of I2C_Bus_Mutex */
  I2C_Bus_MutexHandle = osMutexNew(&I2C_Bus_Mutex_attributes);

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
  /* creation of BLEQueue */
  BLEQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &BLEQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of BLETask */
  BLETaskHandle = osThreadNew(StartBLETask, NULL, &BLETask_attributes);

  /* creation of OLEDTask */
  OLEDTaskHandle = osThreadNew(StartOLEDTask, NULL, &OLEDTask_attributes);

  /* creation of SensorTask */
  SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);

  /* creation of InputTask */
  InputTaskHandle = osThreadNew(StartInputTask, NULL, &InputTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartBLETask */
/**
  * @brief  Function implementing the BLETask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBLETask */
void StartBLETask(void *argument)
{
  /* USER CODE BEGIN StartBLETask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartBLETask */
}

/* USER CODE BEGIN Header_StartOLEDTask */
/**
* @brief Function implementing the OLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOLEDTask */
void StartOLEDTask(void *argument)
{
  /* USER CODE BEGIN StartOLEDTask */
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(I2C_Bus_MutexHandle, osWaitForever);

    OLED_NewFrame();
    OLED_PrintString(0,1,temperatureText);
    OLED_PrintString(0, 2, humidityText);
    OLED_PrintString(0,3, luxText);
    OLED_ShowFrame();

    osMutexRelease(I2C_Bus_MutexHandle);

    osDelay(20);
  }
  /* USER CODE END StartOLEDTask */
}

/* USER CODE BEGIN Header_StartSensorTask */
/**
* @brief Function implementing the SensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorTask */
void StartSensorTask(void *argument)
{
  /* USER CODE BEGIN StartSensorTask */

  float temperature, humidity;
  float lux = 0.0f;
  int luxInt = 0;
  int luxFrac = 0;
  int temperatureInt;
  int temperatureFrac;
  int humidityInt;
  int humidityFrac;

  I2C_Init();
  Light_Init();

  /* Infinite loop */
  for(;;)
  {
    AHT20_Read(&temperature, &humidity);
    Light_ReadLux(&lux);
    FormatValue(lux, &luxInt, &luxFrac);
    FormatValue(temperature, &temperatureInt, &temperatureFrac);
    FormatValue(humidity, &humidityInt, &humidityFrac);

    snprintf(luxText,sizeof(luxText),"Lux: %d.%d lx",luxInt,luxFrac);
    snprintf(temperatureText, sizeof(temperatureText),"Temp: %d.%d C", temperatureInt, temperatureFrac);
    snprintf(humidityText, sizeof(humidityText),"Humi: %d.%d %%", humidityInt, humidityFrac);

    osDelay(1000);
  }
  /* USER CODE END StartSensorTask */
}

/* USER CODE BEGIN Header_StartInputTask */
/**
* @brief Function implementing the InputTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInputTask */
void StartInputTask(void *argument)
{
  /* USER CODE BEGIN StartInputTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartInputTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

