//
// Created by MYlin on 2026/10/13.
//

#ifndef FARMSMARTCODE_SCREEN_H
#define FARMSMARTCODE_SCREEN_H

typedef enum {
    SHOW = 0,
    EDIT
}ScreenState;
extern ScreenState screenState;

typedef enum {
    TEMPERATURE = 0,
    HUMIDITY,
    SOIL_TEMPERATURE,
    ILLUMINANCE,
    PRECIPITATION,
    END
}ScreenType;
extern ScreenType screenType;

void ToggleScreen(void);
void NextScreen(void);
void PrevScreen(void);
#endif //FARMSMARTCODE_SCREEN_H
