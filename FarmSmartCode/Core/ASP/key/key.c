//
// Created by MYlin on 2026/10/17.
//
#include "./key.h"

#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "cmsis_os2.h"

uint8_t isButton_1(void) {
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET) {
        osDelay(10);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET) {

            while (HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET);
            return 1;
        }
    }
    return 0;
}
uint8_t isButton_2(void) {
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_RESET) {
        osDelay(10);
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_RESET) {

            while (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == GPIO_PIN_RESET);
            return 1;
        }
    }
    return 0;
}