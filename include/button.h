#ifndef __BUTTON_H
#define __BUTTON_H

#include <Arduino.h>

// ===== ENUM HƯỚNG =====
typedef enum {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_NONE
} Direction;

// ===== INIT =====
void Button_Init();

// ===== ĐỌC HƯỚNG =====
Direction Button_ReadDirection(Direction currentDir);

// ===== RESET =====
bool Button_IsReset();

#endif