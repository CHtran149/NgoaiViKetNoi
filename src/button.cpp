#include "button.h"

#define BTN_UP    32
#define BTN_DOWN  33
#define BTN_LEFT  25
#define BTN_RIGHT 26
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

Direction Button_ReadDirection(Direction currentDir) {
    // Nếu chưa qua thời gian chống rung, trả về NONE (không làm gì cả)
    if (millis() - lastPressTime < DEBOUNCE_TIME) 
        return DIR_NONE; 

    // Kiểm tra từng nút
    if (digitalRead(BTN_UP) == LOW) {
        lastPressTime = millis();
        return DIR_UP;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
        lastPressTime = millis();
        return DIR_DOWN;
    }
    if (digitalRead(BTN_LEFT) == LOW) {
        lastPressTime = millis();
        return DIR_LEFT;
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
        lastPressTime = millis();
        return DIR_RIGHT;
    }

    // Nếu không nhấn gì cả, PHẢI trả về NONE
    return DIR_NONE; 
}

bool Button_IsReset() {
    if (digitalRead(BTN_RESET) == LOW) {
        delay(150); // Debounce cho nút reset
        return true;
    }
    return false;
}