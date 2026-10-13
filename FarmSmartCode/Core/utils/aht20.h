//
// Created by MYlin on 2026/10/11.
//

#ifndef FARMSMARTCODE_AHT20_H
#define FARMSMARTCODE_AHT20_H
#define AHT20_ADDRESS 0x70

void AHT20_Init(void);
void AHT20_Read(float *temperature, float *humidity);
#endif //FARMSMARTCODE_AHT20_H
