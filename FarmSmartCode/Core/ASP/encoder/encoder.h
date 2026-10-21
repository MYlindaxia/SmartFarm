//
// Created by MYlin on 2026/10/15.
//

#ifndef FARMSMARTCODE_ENCODER_H
#define FARMSMARTCODE_ENCODER_H
typedef enum {
    KNOB_NONE = 0,
    KNOB_RIGHT,
    KNOB_LEFT,
    KNOB_CLICK
}KnobEvent;

void Knob_Init(void);
KnobEvent Knob_isRotating(void);
#endif //FARMSMARTCODE_ENCODER_H
