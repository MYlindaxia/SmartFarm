//
// Created by MYlin on 2026/10/25.
//

#ifndef FARMSMARTCODE_SOIL_H
#define FARMSMARTCODE_SOIL_H
#include "stm32f1xx_hal.h"
#include <stdint.h>

void Soil_Init(void);
uint16_t Soil_Get(void);

extern uint16_t adc_dma_buffer[2];
#endif //FARMSMARTCODE_SOIL_H
