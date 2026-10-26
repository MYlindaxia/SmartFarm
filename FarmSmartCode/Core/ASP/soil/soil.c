//
// Created by MYlin on 2026/10/25.
//
#include "soil.h"
#include "adc.h"
#include "tim.h"
uint16_t adc_dma_buffer[2];

void Soil_Init(void) {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, 2);
    HAL_TIM_Base_Start(&htim3);
}

uint16_t Soil_Get(void) {
    uint16_t value = HAL_ADC_GetValue(&hadc1);
    if (value > 4000) value = 4000;
    return 100 - value / 40;
}