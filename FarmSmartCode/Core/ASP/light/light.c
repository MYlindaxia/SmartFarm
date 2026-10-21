//
// Created by MYlin on 2026/10/19.
//

#include "./light.h"
#include "soft_I2C.h"
#include "cmsis_os2.h"

#define BH1750_WRITE_ADDR  0x46
#define BH1750_READ_ADDR   0x47

#define BH1750_POWER_ON        0x01
#define BH1750_CONT_H_RES      0x10
#define BH1750_ONE_TIME_H_RES  0x20

uint8_t Light_Init(void)
{
    // 发送上电命令
    I2C_Start();

    if (I2C_WriteByte(BH1750_WRITE_ADDR) != 0) {
        I2C_Stop();
        return LIGHT_ERR_ADDRESS_NACK;
    }

    if (I2C_WriteByte(BH1750_POWER_ON) != 0) {
        I2C_Stop();
        return LIGHT_ERR_COMMAND_NACK;
    }

    I2C_Stop();
    osDelay(10);

    // 启动连续高分辨率测量
    I2C_Start();

    if (I2C_WriteByte(BH1750_WRITE_ADDR) != 0) {
        I2C_Stop();
        return LIGHT_ERR_ADDRESS_NACK;
    }

    if (I2C_WriteByte(BH1750_CONT_H_RES) != 0) {
        I2C_Stop();
        return LIGHT_ERR_COMMAND_NACK;
    }

    I2C_Stop();

    // 第一次转换最多需要约 180 ms
    osDelay(200);
    return LIGHT_OK;
}


uint8_t Light_ReadLux(float *lux)
{
    if (lux == NULL) {
        return LIGHT_ERR_ADDRESS_NACK;
    }

    I2C_Start();

    if (I2C_WriteByte(BH1750_READ_ADDR) != 0) {
        I2C_Stop();
        return LIGHT_ERR_ADDRESS_NACK;
    }

    uint8_t high_byte = I2C_ReadByte(0);
    uint8_t low_byte  = I2C_ReadByte(1);

    I2C_Stop();

    uint16_t raw = ((uint16_t)high_byte << 8) | low_byte;
    *lux = raw / 1.2f;

    return LIGHT_OK;
}