//
// Created by MYlin on 2026/10/19.
//

#ifndef FARMSMARTCODE_SOFT_I2C_H
#define FARMSMARTCODE_SOFT_I2C_H

#include "main.h"
#include <stdint.h>

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(uint8_t send_ack);
#endif //FARMSMARTCODE_SOFT_I2C_H
