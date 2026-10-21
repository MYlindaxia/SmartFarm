//
// Created by MYlin on 2026/10/11.
//

#include "./aht20.h"
#include "i2c.h"
#include "cmsis_os2.h"
#include "stm32f1xx_hal_i2c.h"

void AHT20_Init(void) {
    osDelay(40);
    uint8_t Init_state;
    HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &Init_state,1,100);
    if ((Init_state && 0x08) == 0) {
        uint8_t buffer[] = {0xBE,0x08,0x00};
        HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, buffer, 3, 100);
        osDelay(10);
    }
}

void AHT20_Read(float *temperature, float *humidity) {
    uint8_t buffer[] = {0xAC,0x33,0x00};
    HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, buffer, 3, 100);
    osDelay(80);
    uint8_t date[6] = {0};
    HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, date, 6, 100);

    uint32_t raw_humi = ((uint32_t)date[1] << 16 | (uint32_t)date[2] << 8 | date[3]) >> 4;
    uint32_t raw_temp = (((uint32_t)date[3] & 0x0F) << 16) | ((uint32_t)date[4] << 8) | date[5];

    *humidity    = (float)raw_humi * 100.0f / 1048576.0f;        /* 2^20 */
    *temperature = (float)raw_temp * 200.0f / 1048576.0f - 50.0f;
}