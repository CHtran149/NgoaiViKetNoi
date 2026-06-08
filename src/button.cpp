#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "button.h"

#define BTN_UP    32
#define BTN_DOWN  33
#define BTN_LEFT  25
#define BTN_RIGHT 26
#define BTN_RESET 27

static QueueHandle_t buttonQueue = NULL;
static volatile TickType_t lastInterruptTick = 0;
#define DEBOUNCE_TIME_MS 50

static void IRAM_ATTR Button_SendEventFromISR(ButtonEventType eventType, Direction direction)
{
    if (buttonQueue == NULL) {
        return;
    }

    TickType_t currentTick = xTaskGetTickCountFromISR();
    if ((currentTick - lastInterruptTick) < pdMS_TO_TICKS(DEBOUNCE_TIME_MS)) {
        return;
    }
    lastInterruptTick = currentTick;

    ButtonEvent event = { .type = eventType, .direction = direction };
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

static void IRAM_ATTR Button_UpISR() {
    Button_SendEventFromISR(BUTTON_TYPE_DIRECTION, DIR_UP);
}

static void IRAM_ATTR Button_DownISR() {
    Button_SendEventFromISR(BUTTON_TYPE_DIRECTION, DIR_DOWN);
}

static void IRAM_ATTR Button_LeftISR() {
    Button_SendEventFromISR(BUTTON_TYPE_DIRECTION, DIR_LEFT);
}

static void IRAM_ATTR Button_RightISR() {
    Button_SendEventFromISR(BUTTON_TYPE_DIRECTION, DIR_RIGHT);
}

static void IRAM_ATTR Button_ResetISR() {
    Button_SendEventFromISR(BUTTON_TYPE_RESET, DIR_NONE);
}

void Button_Init() {
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pinMode(BTN_RESET, INPUT_PULLUP);
}

void Button_SetupInterrupts(QueueHandle_t queue) {
    buttonQueue = queue;
    attachInterrupt(digitalPinToInterrupt(BTN_UP), Button_UpISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_DOWN), Button_DownISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_LEFT), Button_LeftISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_RIGHT), Button_RightISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_RESET), Button_ResetISR, FALLING);
}

Direction Button_ReadDirection() {
    if (digitalRead(BTN_UP) == LOW) {
        return DIR_UP;
    } else if (digitalRead(BTN_DOWN) == LOW) {
        return DIR_DOWN;
    } else if (digitalRead(BTN_LEFT) == LOW) {
        return DIR_LEFT;
    } else if (digitalRead(BTN_RIGHT) == LOW) {
        return DIR_RIGHT;
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
