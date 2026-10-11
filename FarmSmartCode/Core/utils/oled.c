//
// Created by MYlin on 2026/10/11.
//

#include "cmsis_os2.h"
#include "i2c.h"
#include "stm32f1xx_hal_i2c.h"
#include "oled.h"
#include "font.h"
#include <stdint.h>
#include <string.h>


uint8_t OLED_GRAM[OLED_PAGES][OLED_WIDTH];
void OLED_Init(void) {
    osDelay(100);
    uint8_t init_cmds[] = {
        0xAE,           /* 关显示 */
        0x02,           /* 低列地址 = 2  —— 128 面板在 132 列显存里的偏移 */
        0x10,           /* 高列地址 = 0 */
        0x40,           /* 显示起始行 = 0 */
        0xB0,           /* 页地址 = 0 */
        0x81, 0xFF,     /* 对比度 = 255 */
        0xA1,           /* 段重映射 SEG0 → 第127列 */
        0xA6,           /* 正常显示（非反白） */
        0xA8, 0x3F,     /* 多路复用 = 1/64 */
        0xAD, 0x8B,     /* 开内部 DC-DC */
        0x33,           /* 内部电荷泵 VPP 档位 */
        0xC8,           /* COM 扫描方向反向 */
        0xD3, 0x00,     /* 显示偏移 = 0 */
        0xD5, 0x80,     /* 时钟分频 / 振荡频率 */
        0xD9, 0x1F,     /* 预充电周期（内部电荷泵） */
        0xDA, 0x12,     /* COM 引脚配置 */
        0xDB, 0x40,     /* VCOMH 去选电平 */
        0xAF            /* 开显示 */
    };
    OLED_WriteCmdList(init_cmds,sizeof(init_cmds));
}

HAL_StatusTypeDef OLED_WriteCmdList(uint8_t *cmds,uint16_t len) {
    return HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, cmds, len, 100);
}

HAL_StatusTypeDef OLED_WriteData(uint8_t *data, uint16_t len) {
    return HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, data, len, 100);
}

HAL_StatusTypeDef OLED_WriteCmd(uint8_t cmd) {
    return HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
}


void OLED_NewFrame(void) {
    memset(OLED_GRAM,0x00,sizeof(OLED_GRAM));
}

void OLED_ShowFrame(void) {
    for (uint8_t page = 0;page<OLED_PAGES;page++) {
        OLED_WriteCmd(0xB0 | page);   /* 选页 */
        OLED_WriteCmd(0x02);          /* 列起始低 4 位 = 2 */
        OLED_WriteCmd(0x10);          /* 列起始高 4 位 = 0 */
        OLED_WriteData(OLED_GRAM[page], OLED_WIDTH);
    }
}

void OLED_SetPixel(int8_t x, int8_t y, OLED_color color)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    uint8_t page = y / 8;
    uint8_t bit  = y % 8;

    if (color == OLED_WHITE)
    {
        OLED_GRAM[page][x] |=  (uint8_t)(1u << bit);
    }
    else
    {
        OLED_GRAM[page][x] &= (uint8_t)~(1u << bit);
    }
}

void OLED_PrintString(uint8_t x, uint8_t page, const char *str)
{
    if (page >= OLED_PAGES)
    {
        return;
    }

    while (*str != '\0')
    {
        uint8_t c = (uint8_t)*str++;

        if (c < FONT6X8_FIRST || c > FONT6X8_LAST)
        {
            c = ' ';
        }

        const uint8_t *glyph = ascii_6x8[c - FONT6X8_FIRST];

        for (uint8_t i = 0; i < FONT6X8_W; i++)
        {
            if (x + i >= OLED_WIDTH)
            {
                return;               /* 写到屏幕右边就停 */
            }
            OLED_GRAM[page][x + i] = glyph[i];
        }

        x += FONT6X8_W;
    }
}


