//
// Created by MYlin on 2026/10/13.
//
#include "screen.h"
ScreenState screenState;
ScreenType screenType = TEMPERATURE;

void ToggleScreen() {
    if (screenState == SHOW) {
        screenState = EDIT;
    }else {
        screenState = SHOW;
    }
}
void NextScreen() {
    screenType++;
    if (screenType == END) {
        screenType = TEMPERATURE;
    }
}
void PrevScreen() {
    screenType--;
    if (screenType == TEMPERATURE) {
        screenType = PRECIPITATION;
    }
}