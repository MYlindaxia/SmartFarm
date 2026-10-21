//
// Created by MYlin on 2026/10/15.
//

#include "./encoder.h"

#include "cmsis_os2.h"
#include "tim.h"

#define COUNTER_INIT_VALUE (65535/2)

void Knob_SetCounter(void) {
    __HAL_TIM_SET_COUNTER(&htim1, COUNTER_INIT_VALUE);
}

uint32_t Knob_GetCounter(void) {
    return __HAL_TIM_GET_COUNTER(&htim1);
}

void Knob_Init(void) {
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    Knob_SetCounter();
}


KnobEvent Knob_isRotating(void) {
    uint32_t counter = Knob_GetCounter();
    KnobEvent direction = KNOB_NONE;

    if (counter > COUNTER_INIT_VALUE) {
        direction = KNOB_RIGHT;
    }else if (counter < COUNTER_INIT_VALUE) {
        direction = KNOB_LEFT;
    }
    if (HAL_GPIO_ReadPin(ENV_KEY_GPIO_Port, ENV_KEY_Pin) == GPIO_PIN_RESET) {
        osDelay(20);
        if (HAL_GPIO_ReadPin(ENV_KEY_GPIO_Port, ENV_KEY_Pin) == GPIO_PIN_RESET) {
            direction = KNOB_CLICK;
            while (HAL_GPIO_ReadPin(ENV_KEY_GPIO_Port, ENV_KEY_Pin) == GPIO_PIN_RESET);
        }
    }
    Knob_SetCounter();
    return direction;
}