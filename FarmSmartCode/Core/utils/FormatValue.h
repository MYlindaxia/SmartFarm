//
// Created by MYlin on 2026/10/14.
//

#ifndef FARMSMARTCODE_FORMATVALUE_H
#define FARMSMARTCODE_FORMATVALUE_H
#include <stddef.h>
#include <stdint.h>


void FormatValue(float v, int *p_int, int *p_frac);
int32_t StringToTempX10(const char* str);
#endif //FARMSMARTCODE_FORMATVALUE_H
