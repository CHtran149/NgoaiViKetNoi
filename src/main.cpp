#include <Arduino.h>
#include "bientoancuc.h"
#include "button.h"
#include "quetled.h"
#include "snake.h"

#define SIPO_DS   23
#define SIPO_SHCP 18
#define SIPO_STCP 5

void setup() {
    Button_Init();
    QuetLed_Init(SIPO_DS, SIPO_SHCP, SIPO_STCP);
    Snake_Init();
}

void loop() {
    Direction currentInput = Button_ReadDirection();
    if (currentInput != DIR_NONE) {
        Snake_SetDirection(currentInput);
    }

    Snake_Update();

    if (Button_IsReset()) {
        Snake_Reset();
    }

    QuetLed_Matrix();
}
