//
// Created by MYlin on 2026/10/11.
//

#ifndef FARMSMARTCODE_FONT_H
#define FARMSMARTCODE_FONT_H
#include <stdint.h>

/* 6x8 ASCII 字库
 * 覆盖 0x20(空格) ~ 0x7E(~)，共 95 个字符
 * 每个字符 6 字节 = 6 列；每字节 8 位 = 这一列从上到下的 8 行
 * bit0 = 最上面一行, bit7 = 最下面一行
 * 查表下标 = 字符的 ASCII 码 - 0x20
 */
#define FONT6X8_W     6
#define FONT6X8_H     8
#define FONT6X8_FIRST 0x20
#define FONT6X8_LAST  0x7E
#define FONT6X8_COUNT (FONT6X8_LAST - FONT6X8_FIRST + 1)

extern const uint8_t ascii_6x8[FONT6X8_COUNT][FONT6X8_W];

/* 额外符号（不是 ASCII 字符，单独一张小表） */
enum {
    SYM_DEGREE = 0,   /* ° 摄氏度 */
    SYM_COUNT
};

extern const uint8_t symbols_6x8[SYM_COUNT][FONT6X8_W];
#endif //FARMSMARTCODE_FONT_H
