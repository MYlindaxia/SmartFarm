//
// Created by MYlin on 2026/10/19.
//
#include "soft_I2C.h"
#include "cmsis_os2.h"

#define SCL_LOW()  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)
#define SCL_HIGH() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define SCL_RELEASE() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define SDA_LOW() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SDA_RELEASE() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)

#define SOFT_I2C_PORT GPIOA

/*
 * 判断数据线是否为高电平
 * 0表示低电平 1表示高电平
 */
uint8_t SDA_IS_HIGH(void) {
    if (HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin) == GPIO_PIN_RESET) {
        return 0;
    }else {
        return 1;
    }
}

/*
 * 延迟1毫秒
 */
void SoftI2C_Delay(void)
{
    osDelay(1);
}

/*
 * I2C初始化
 * 将SDA、SCL全置为高电平
 *
 */
void I2C_Init(void) {
    SDA_HIGH();
    SCL_HIGH();
    osDelay(5);
}

/*
 * I2C开始启动
 * 在时钟线为高电平的时候将数据线拉为低电平，I2C开始启动，并且将时钟线拉低，方便数据线高低电平变化。
 */
void I2C_Start(void) {
    SDA_HIGH();
    SCL_HIGH();
    SoftI2C_Delay();
    SDA_LOW();
    SoftI2C_Delay();
    SCL_LOW();
    SoftI2C_Delay();
}

/*
 * I2C停止
 * 先将数据线置为低电平，然后将时钟线置为高电平，在时钟线为高电平的时候将数据线置为高电平，通信结束。
 */
void I2C_Stop(void) {
    SDA_LOW();
    SoftI2C_Delay();

    SCL_HIGH();
    SoftI2C_Delay();

    SDA_HIGH();
    SoftI2C_Delay();
}

/*
 * 发送bit数据
 * 先将时钟线置为0，如果bit_value不为0则将数据线置为高电平，否则为低电平，最后将时钟线置为高电平让从机读取数据。
 */
void SoftI2C_WriteBit(uint8_t bit_value) {
    SCL_LOW();
    if (bit_value != 0) {
        SDA_HIGH();
    }else {
        SDA_LOW();
    }
    SoftI2C_Delay();
    SCL_HIGH();
    SoftI2C_Delay();
    SCL_LOW();
    SoftI2C_Delay();
}


uint8_t I2C_WriteByte(uint8_t data) {
    for (uint8_t i = 0;i<8;i++) {
        if ((data & 0x80) != 0) {
            SoftI2C_WriteBit(1);
        }else {
            SoftI2C_WriteBit(0);
        }
        data <<= 1;
    }

    SDA_HIGH();
    SoftI2C_Delay();
    SCL_HIGH();
    SoftI2C_Delay();

    uint8_t ack = SDA_IS_HIGH() ? 1 : 0;
    SCL_LOW();
    SoftI2C_Delay();

    return ack;
}

uint8_t I2C_ReadBit(void) {
    SDA_HIGH();
    SoftI2C_Delay();
    SCL_HIGH();
    SoftI2C_Delay();
    uint8_t bit = SDA_IS_HIGH();
    SCL_LOW();
    SoftI2C_Delay();
    return bit;
}

uint8_t I2C_ReadByte(uint8_t send_ack) {
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        data |= I2C_ReadBit();
    }

    if (send_ack == 0) {
        SDA_LOW();       // 发送 ACK
    } else {
        SDA_HIGH();   // 发送 NACK
    }

    SoftI2C_Delay();
    SCL_HIGH();
    SoftI2C_Delay();
    SCL_LOW();
    SoftI2C_Delay();
    SDA_HIGH();

    return data;
}