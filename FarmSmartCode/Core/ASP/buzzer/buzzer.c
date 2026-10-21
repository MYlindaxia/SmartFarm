//
// Created by MYlin on 2026/10/15.
//
#include "./buzzer.h"
#include "tim.h"
#include "stm32f1xx_hal_tim.h"

void Buzzer_On(void) {
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 500);
}
void Buzzer_Off(void) {
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
}