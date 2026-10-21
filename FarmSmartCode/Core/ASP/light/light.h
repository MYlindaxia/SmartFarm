//
// Created by MYlin on 2026/10/19.
//
#include <stdint.h>

#ifndef FARMSMARTCODE_LIGHT_H
#define FARMSMARTCODE_LIGHT_H

#define LIGHT_OK 0U
#define LIGHT_ERR_ADDRESS_NACK 1U
#define LIGHT_ERR_COMMAND_NACK 2U

uint8_t Light_Init(void);
uint8_t Light_ReadLux(float *lux);
#endif //FARMSMARTCODE_LIGHT_H
