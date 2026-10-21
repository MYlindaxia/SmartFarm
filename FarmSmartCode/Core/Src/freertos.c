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
#include "../per/oled.h"
#include "../per/aht20.h"
#include <stdio.h>
#include "../per/encoder.h"
#include "../per/light.h"
#include "../per/ble.h"
#include "../utils/FormatValue.h"
#include "../per/buzzer.h"
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
uint8_t ack;
uint8_t count = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
char msg[64];
int temperatureInt;
int humidityInt;
int temperatureFrac;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
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
/* Definitions for AHT20Task */
osThreadId_t AHT20TaskHandle;
const osThreadAttr_t AHT20Task_attributes = {
  .name = "AHT20Task",
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

void StartDefaultTask(void *argument);
void StartOLEDTask(void *argument);
void StartAHT20Task(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of OLEDTask */
  OLEDTaskHandle = osThreadNew(StartOLEDTask, NULL, &OLEDTask_attributes);

  /* creation of AHT20Task */
  AHT20TaskHandle = osThreadNew(StartAHT20Task, NULL, &AHT20Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
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
  OLED_Init();
  AHT20_Init();
  Knob_Init();
  I2C_Init();
  /* Infinite loop */
  for(;;)
  {

    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
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
  (void)argument;

  char luxText[32];
  float lux = 0.0f;
  int luxInt = 0;
  int luxFrac = 0;
  uint8_t lightState;

  I2C_Init();
  lightState = Light_Init();
  /* Infinite loop */
  for(;;)
  {

    if (lightState == LIGHT_OK) {
      lightState = Light_ReadLux(&lux);
    }

    if (lightState == LIGHT_OK) {
      FormatValue(lux, &luxInt, &luxFrac);

      snprintf(luxText,
               sizeof(luxText),
               "Lux: %d.%d lx",
               luxInt,
               luxFrac);
    } else {
      snprintf(luxText,
               sizeof(luxText),
               "Light Error: %d",
               lightState);
    }

    osMutexAcquire(I2C_Bus_MutexHandle, osWaitForever);

    OLED_NewFrame();
    OLED_PrintString(0, 0, "Light Sensor");
    OLED_PrintString(0, 2, luxText);
    OLED_ShowFrame();

    osMutexRelease(I2C_Bus_MutexHandle);

    osDelay(500);
  }
  /* USER CODE END StartOLEDTask */
}

/* USER CODE BEGIN Header_StartAHT20Task */
/**
* @brief Function implementing the AHT20Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAHT20Task */
void StartAHT20Task(void *argument)
{
  /* USER CODE BEGIN StartAHT20Task */
  float temperature, humidity;
  /* Infinite loop */
  for(;;)
  {

    // osMutexAcquire(I2C_Bus_MutexHandle,osWaitForever);
    // AHT20_Read(&temperature, &humidity);
    //
    // FormatValue(temperature,&temperatureInt,&temperatureFrac);
    // FormatValue(humidity,&humidityInt,NULL);
    // snprintf(msg, sizeof(msg), "%d.%d", temperatureInt,temperatureFrac);   /* 25 -> "25" */
    // osMutexRelease(I2C_Bus_MutexHandle);


    osDelay(1000);
  }
  /* USER CODE END StartAHT20Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

