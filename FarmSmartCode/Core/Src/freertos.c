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
#include "../utils/FormatValue.h"
#include "../ASP/buzzer/buzzer.h"
#include "../utils/soft_I2C.h"
#include "../ASP/key/key.h"
#include "../configuration/screen.h"
#include "../configuration/environment.h"
#include "../ASP/ble/ble.h"
#include "../ASP/soil/soil.h"

#define OK 0
#define TEMP_ERROR 1
#define HUM_ERROR 2
#define LUX_ERROR 3
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t count = 0;
char luxText[32];
char temperatureText[32];
char humidityText[32];
char soilText[32];
KnobEvent direction;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
char msg[64];
/* USER CODE END Variables */
/* Definitions for AlterTask */
osThreadId_t AlterTaskHandle;
const osThreadAttr_t AlterTask_attributes = {
  .name = "AlterTask",
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
void ShowScreen(void) {
    OLED_PrintString(0,0,(char*)"Show:");
    OLED_PrintString(0,1,temperatureText);
    OLED_PrintString(0, 2, humidityText);
    OLED_PrintString(0,3, luxText);
    OLED_PrintString(0,4,soilText);
}

char isChange(uint8_t state) {
  if (count == state) {
    return '*';
  }else {
    return ' ';
  }
}

void updateUserState(void) {
    if (screenState == EDIT) {
      switch (count) {
        case 0:
            /* 尚未选中可编辑字段，不处理旋转 */
            break;

        case 1: /* 最大温度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.temperature.max - 1.0f >=
                        userSetState.temperature.min) {
                        userSetState.temperature.max -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.temperature.max + 1.0f <=
                        farmSafeState.MAX_TEMPERATURE) {
                        userSetState.temperature.max += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 2: /* 最小温度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.temperature.min - 1.0f >=
                        farmSafeState.MIN_TEMPERATURE) {
                        userSetState.temperature.min -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.temperature.min + 1.0f <=
                        userSetState.temperature.max) {
                        userSetState.temperature.min += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 3: /* 最大湿度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.humidity.max - 1.0f >=
                        userSetState.humidity.min) {
                        userSetState.humidity.max -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.humidity.max + 1.0f <=
                        farmSafeState.MAX_HUMIDITY) {
                        userSetState.humidity.max += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 4: /* 最小湿度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.humidity.min - 1.0f >=
                        farmSafeState.MIN_HUMIDITY) {
                        userSetState.humidity.min -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.humidity.min + 1.0f <=
                        userSetState.humidity.max) {
                        userSetState.humidity.min += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 5: /* 最大光照强度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.illuminance.max - 1.0f >=
                        userSetState.illuminance.min) {
                        userSetState.illuminance.max -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.illuminance.max + 1.0f <=
                        farmSafeState.MAX_Illuminance) {
                        userSetState.illuminance.max += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 6: /* 最小光照强度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.illuminance.min - 1.0f >=
                        farmSafeState.MIN_Illuminance) {
                        userSetState.illuminance.min -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.illuminance.min + 1.0f <=
                        userSetState.illuminance.max) {
                        userSetState.illuminance.min += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 7: /* 最大降雨量 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.precipitation.max - 1.0f >=
                        userSetState.precipitation.min) {
                        userSetState.precipitation.max -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.precipitation.max + 1.0f <=
                        farmSafeState.MAX_Precipitation) {
                        userSetState.precipitation.max += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 8: /* 最小降雨量 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.precipitation.min - 1.0f >=
                        farmSafeState.MIN_Precipitation) {
                        userSetState.precipitation.min -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.precipitation.min + 1.0f <=
                        userSetState.precipitation.max) {
                        userSetState.precipitation.min += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 9: /* 最大土壤温度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.soilHumidity.max - 1.0f >=
                        userSetState.soilHumidity.min) {
                        userSetState.soilHumidity.max -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.soilHumidity.max + 1.0f <=
                        farmSafeState.MAX_SoilHUMIDITY) {
                        userSetState.soilHumidity.max += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 10: /* 最小土壤温度 */
            switch (direction) {
                case KNOB_RIGHT:
                    if (userSetState.soilHumidity.min - 1.0f >=
                        farmSafeState.MAX_SoilHUMIDITY) {
                        userSetState.soilHumidity.min -= 1.0f;
                    }
                    break;

                case KNOB_LEFT:
                    if (userSetState.soilHumidity.min + 1.0f <=
                        userSetState.soilHumidity.max) {
                        userSetState.soilHumidity.min += 1.0f;
                    }
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
    }
}

void EditScreen(void) {
  char Text[32];
  int intNum1;
  int fracNum1;
  int intNum2;
  int fracNum2;

  OLED_PrintString(0,0,(char*)"Edit:");
  FormatValue(userSetState.temperature.max, &intNum1, &fracNum1);
  FormatValue(userSetState.temperature.min, &intNum2, &fracNum2);
  snprintf(Text, sizeof(Text), "%cT: %d.%d%cT: %d.%d",isChange(1), intNum1, fracNum1, isChange(2),intNum2, fracNum2);
  // snprintf(Text, sizeof(Text), "Temp %d.%d, Temp %d.%d",intNum1,fracNum1, intNum2, fracNum2);
  OLED_PrintString(0,1,Text);

  FormatValue(userSetState.humidity.max, &intNum1, &fracNum1);
  FormatValue(userSetState.humidity.min, &intNum2, &fracNum2);
  snprintf(Text, sizeof(Text), "%cH: %d.%d%cH: %d.%d",isChange(3),intNum1,fracNum1,isChange(4), intNum2, fracNum2);
  OLED_PrintString(0,2,Text);

  FormatValue(userSetState.illuminance.max, &intNum1, &fracNum1);
  FormatValue(userSetState.illuminance.min, &intNum2, &fracNum2);
  snprintf(Text, sizeof(Text), "%cI: %d.%d%cI: %d.%d",isChange(5),intNum1,fracNum1,isChange(6), intNum2, fracNum2);
  OLED_PrintString(0,3,Text);

  FormatValue(userSetState.precipitation.max, &intNum1, &fracNum1);
  FormatValue(userSetState.precipitation.min, &intNum2, &fracNum2);
  snprintf(Text, sizeof(Text), "%cP: %d.%d%cP: %d.%d",isChange(7),intNum1,fracNum1,isChange(8), intNum2, fracNum2);
  OLED_PrintString(0,4,Text);

  FormatValue(userSetState.soilHumidity.max, &intNum1, &fracNum1);
  FormatValue(userSetState.soilHumidity.min, &intNum2, &fracNum2);
  snprintf(Text, sizeof(Text), "%cS: %d.%d%cS: %d.%d",isChange(9),intNum1,fracNum1,isChange(10), intNum2, fracNum2);
  OLED_PrintString(0,5,Text);
}

uint8_t checkMsg(UserSetState userSetState) {
    if (StringToTempX10(temperatureText) > (int32_t)(userSetState.temperature.max * 10) || StringToTempX10(temperatureText) < (int32_t)(userSetState.temperature.min*10)) {
        return TEMP_ERROR;
    }else if (StringToTempX10(humidityText) > (int32_t)(userSetState.humidity.max * 10) || StringToTempX10(humidityText) < (int32_t)(userSetState.humidity.min*10)) {
        return HUM_ERROR;
    }else if (StringToTempX10(luxText) > (int32_t)(userSetState.illuminance.max*10) || StringToTempX10(luxText) < (int32_t)(userSetState.illuminance.min*10)) {
        return LUX_ERROR;
    }
    return OK;
}
/* USER CODE END FunctionPrototypes */

void StartAlterTask(void *argument);
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
  /* creation of AlterTask */
  AlterTaskHandle = osThreadNew(StartAlterTask, NULL, &AlterTask_attributes);

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

/* USER CODE BEGIN Header_StartAlterTask */
/**
  * @brief  Function implementing the AlterTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartAlterTask */
void StartAlterTask(void *argument)
{
  /* USER CODE BEGIN StartAlterTask */
    uint8_t error_msg = OK;
  /* Infinite loop */
  for(;;)
  {

    error_msg = checkMsg(userSetState);
    if (error_msg != OK) {
        // Buzzer_On();
        HAL_UART_Transmit(&huart2,(uint8_t*)"ERROR\n",6,1000);
        osMessageQueuePut(BLEQueueHandle, &error_msg, 0, 100);
        StartBLE();
    }
    osDelay(1000);
  }
  /* USER CODE END StartAlterTask */
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
  OLED_Init();
  InitFarmSafeState();
  InitUserSetState();
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(I2C_Bus_MutexHandle, osWaitForever);

    OLED_NewFrame();
    if (screenState == SHOW) {
      ShowScreen();
    }else {
      EditScreen();
    }
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
  uint32_t soil;
  uint32_t rain;
  int intNum;
  int fracNum;


  I2C_Init();
  Light_Init();
  Soil_Init();

  /* Infinite loop */
  for(;;)
  {
    // 开始传感器采集
    osMutexAcquire(I2C_Bus_MutexHandle, osWaitForever);
    AHT20_Read(&temperature, &humidity);
    Light_ReadLux(&lux);
    soil = adc_dma_buffer[0];
    rain = adc_dma_buffer[1];
    if (soil > 4000) soil = 4000;
    soil =  100 - soil / 40;
    if (rain > 4000) rain = 4000;
    rain =  100 - rain / 40;
    FormatValue(lux, &intNum, &fracNum);
    snprintf(luxText,sizeof(luxText),"Lux: %d.%d lx",intNum,fracNum);

    FormatValue(temperature, &intNum, &fracNum);
    snprintf(temperatureText, sizeof(temperatureText),"Temp: %d.%d C", intNum, fracNum);

    FormatValue(humidity, &intNum, &fracNum);
    snprintf(humidityText, sizeof(humidityText),"Humi: %d.%d %%", intNum, fracNum);

    snprintf(soilText,sizeof(soilText), "Soil: %d rain: %d", soil, rain);

    osMutexRelease(I2C_Bus_MutexHandle);

    // 检测参数是否过高/过低

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
  Knob_Init();
  /* Infinite loop */
  for(;;)
  {
    uint8_t changeIsBtn = isButton_1();
    uint8_t envIsBtn = EnvIsBtn();
    direction = Knob_isRotating();
    updateUserState();
    if (changeIsBtn == 1) {
      ToggleScreen();
    }else {
      osDelay(1);
    }

    if (envIsBtn == 1) {
      if (count <= 9) count++;
      else count=0;
      osDelay(1);
    }else {
      osDelay(1);
    }
    osDelay(10);
  }
  /* USER CODE END StartInputTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

