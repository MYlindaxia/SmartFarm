//
// Created by MYlin on 2026/10/17.
//


#include <string.h>
#include "cmsis_os.h"
#include "usart.h"
#include <stdio.h>
extern osMessageQueueId_t BLEQueueHandle;

void StartBLE(void) {
    uint8_t value;
    char buf[16];
    if (osMessageQueueGet(BLEQueueHandle, &value, NULL,0) != osOK) return;
    int len = snprintf(buf, sizeof(buf), "count=%u\r\n", value);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, len,100);
}
