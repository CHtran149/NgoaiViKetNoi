#ifndef __BUTTON_H
#define __BUTTON_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// ===== ENUM HƯỚNG =====
typedef enum {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_NONE
} Direction;

typedef enum {
    BUTTON_TYPE_DIRECTION = 0,
    BUTTON_TYPE_RESET
} ButtonEventType;

typedef struct {
    ButtonEventType type;
    Direction direction;
} ButtonEvent;

// ===== INIT =====
void Button_Init();
void Button_SetupInterrupts(QueueHandle_t buttonQueue);

// ===== ĐỌC HƯỚNG =====
Direction Button_ReadDirection();

// ===== RESET =====
bool Button_IsReset();

#endif