#include "button.h"

#define BTN_UP    32
#define BTN_DOWN  33
#define BTN_LEFT  26
#define BTN_RIGHT 25
#define BTN_RESET 27

static unsigned long lastPressTime = 0;
#define DEBOUNCE_TIME 200 // Tăng lên một chút nếu nút bấm bị lỏng

void Button_Init() {
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pinMode(BTN_RESET, INPUT_PULLUP);
}

Direction Button_ReadDirection() {
    static Direction lastDirection = DIR_NONE;
    Direction currentDirection = DIR_NONE;

    if (digitalRead(BTN_UP) == LOW) {
        currentDirection = DIR_UP;
    } else if (digitalRead(BTN_DOWN) == LOW) {
        currentDirection = DIR_DOWN;
    } else if (digitalRead(BTN_LEFT) == LOW) {
        currentDirection = DIR_LEFT;
    } else if (digitalRead(BTN_RIGHT) == LOW) {
        currentDirection = DIR_RIGHT;
    }

    if (currentDirection == DIR_NONE) {
        lastDirection = DIR_NONE;
        return DIR_NONE;
    }

    if (currentDirection != lastDirection || millis() - lastPressTime > DEBOUNCE_TIME) {
        lastPressTime = millis();
        lastDirection = currentDirection;
        return currentDirection;
    }

    return DIR_NONE;
}

bool Button_IsReset() {
    if (digitalRead(BTN_RESET) == LOW) {
        delay(150); // Debounce cho nút reset
        return true;
    }
    return false;
}