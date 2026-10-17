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
