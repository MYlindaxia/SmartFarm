//
// Created by MYlin on 2026/10/17.
//


#include <string.h>
#include "cmsis_os.h"
#include "usart.h"
#include <stdio.h>
extern osMessageQueueId_t BLEQueueHandle;

void StartBLE(void) {
    // char *msg;
    // osMessageQueueGet(BLEQueueHandle,&msg,NULL,osWaitForever);
    // if (msg != NULL) {
    //     HAL_UART_Transmit_DMA(&huart3, (uint8_t*)msg, strlen(msg));
    //
    //     while (1) {
    //         if (HAL_UART_GetState(&huart3) == HAL_UART_STATE_RESET) {
    //             break;
    //         }
    //         osDelay(1);
    //     }
    //     vPortFree(msg);
    // }
    uint8_t value;
    char buf[16];
    if (osMessageQueueGet(BLEQueueHandle, &value, NULL,0) != osOK) return;
    int len = snprintf(buf, sizeof(buf), "count=%u\r\n", value);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, len,100);
}
