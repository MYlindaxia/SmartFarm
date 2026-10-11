//
// Created by MYlin on 2026/10/11.
//

#ifndef FARMSMARTCODE_OLED_H
#define FARMSMARTCODE_OLED_H

#define OLED_ADDRESS 0x7A
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGES 8
typedef enum {
    OLED_BLACK = 0,
    OLED_WHITE = 1
}OLED_color;
extern uint8_t OLED_GRAM[OLED_PAGES][OLED_WIDTH];

void OLED_Init(void);
HAL_StatusTypeDef OLED_WriteCmd(uint8_t cmd);
HAL_StatusTypeDef OLED_WriteCmdList(uint8_t *cmds,uint16_t len);
HAL_StatusTypeDef OLED_WriteData(uint8_t *data, uint16_t len);

void OLED_NewFrame(void);
void OLED_ShowFrame(void);
void OLED_SetPixel(int8_t x, int8_t y, OLED_color color);
void OLED_PrintString(uint8_t x, uint8_t page, const char *str);


#endif //FARMSMARTCODE_OLED_H
