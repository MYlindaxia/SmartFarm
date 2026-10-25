//
// Created by MYlin on 2026/10/14.
//
#include "FormatValue.h"
#include <stdio.h>

void FormatValue(float v, int *p_int, int *p_frac) {
    int negative = (v < 0.0f);
    float a = negative ? -v : v;
    int scaled = (int)(a * 10.0f + 0.5f);   /* ×10 并四舍五入到 0.1 */

    *p_int  = scaled / 10;              /* 整数部分 */
    if (p_frac != NULL) {
        *p_frac = scaled % 10;             /* 小数位，一定是 0~9 */
    }

    if (negative) {
        *p_int = -*p_int;              /* 符号补回到整数部分 */
    }
}

int32_t StringToTempX10(const char* str) {
    int32_t val = 0;
    uint8_t is_decimal = 0;
    int32_t decimal_mult = 1;

    while (*str) {
        if (*str >= '0' && *str <= '9') {
            if (!is_decimal) {
                val = val * 10 + (*str - '0');
            } else if (decimal_mult < 10) { // 只取一位小数
                val = val * 10 + (*str - '0');
                decimal_mult *= 10;
            }
        } else if (*str == '.') {
            is_decimal = 1;
            // 如果没有整数部分（比如 .5），补 0
            if (val == 0) val = 0;
        }
        str++;
    }
    return val;
}
