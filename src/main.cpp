#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "bientoancuc.h"
#include "button.h"
#include "quetled.h"
#include "snake.h"
#include "CLCD.h"

#define SIPO_DS   23
#define SIPO_SHCP 18
#define SIPO_STCP 5
#define LCD_RS    12
#define LCD_EN    13
#define LCD_DS    14
#define LCD_SHCP  15
#define LCD_STCP  16

static QueueHandle_t buttonQueue = NULL;
static QueueHandle_t scoreQueue = NULL;
static SemaphoreHandle_t stateMutex = NULL;  // Mutex bảo vệ requestedDirection, requestedReset, lastResetTime
static CLCD_Name lcd;
static Direction requestedDirection = DIR_NONE;
static bool requestedReset = false;
static unsigned long lastResetTime = 0;
#define RESET_COOLDOWN_MS 300

static void TaskLCD(void *pvParameters)
{
    int currentScore = 0;
    char buffer[17];

    CLCD_Clear(&lcd);
    CLCD_SetCursor(&lcd, 0, 0);
    CLCD_WriteString(&lcd, "Nhom2_NVGN");

    CLCD_SetCursor(&lcd, 0, 1);
    snprintf(buffer, sizeof(buffer), "Scores: %d", currentScore);
    CLCD_WriteString(&lcd, buffer);

    for (;;) {
        int newScore;
        if (xQueueReceive(scoreQueue, &newScore, pdMS_TO_TICKS(200)) == pdTRUE) {
            currentScore = newScore;
            CLCD_SetCursor(&lcd, 0, 1);
            snprintf(buffer, sizeof(buffer), "Scores: %d   ", currentScore);
            CLCD_WriteString(&lcd, buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void TaskButton(void *pvParameters)
{
    ButtonEvent event;
    for (;;) {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY) == pdTRUE) {
            if (event.type == BUTTON_TYPE_DIRECTION) {
                xSemaphoreTake(stateMutex, portMAX_DELAY);
                requestedDirection = event.direction;
                xSemaphoreGive(stateMutex);
            } else if (event.type == BUTTON_TYPE_RESET) {
                unsigned long now = millis();
                xSemaphoreTake(stateMutex, portMAX_DELAY);
                if (now - lastResetTime >= RESET_COOLDOWN_MS) {
                    requestedReset = true;
                    lastResetTime = now;
                }
                xSemaphoreGive(stateMutex);
            }
        }
    }
}

static void TaskSnake(void *pvParameters)
{
    int lastScore = Snake_GetScore();
    xQueueOverwrite(scoreQueue, &lastScore);

    for (;;) {
        xSemaphoreTake(stateMutex, portMAX_DELAY);
        Direction dirToSet = requestedDirection;
        bool needReset = requestedReset;
        if (requestedDirection != DIR_NONE) {
            requestedDirection = DIR_NONE;
        }
        if (requestedReset) {
            requestedReset = false;
        }
        xSemaphoreGive(stateMutex);

        if (dirToSet != DIR_NONE) {
            Snake_SetDirection(dirToSet);
        }

        if (needReset) {
            Snake_Reset();
            lastScore = Snake_GetScore();
            xQueueOverwrite(scoreQueue, &lastScore);
        }

        Snake_Update();

        int currentScore = Snake_GetScore();
        if (currentScore != lastScore) {
            lastScore = currentScore;
            xQueueOverwrite(scoreQueue, &lastScore);
        }

        QuetLed_Matrix();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setup() {
    Button_Init();
    QuetLed_Init(SIPO_DS, SIPO_SHCP, SIPO_STCP);
    CLCD_595_8BIT_Init(&lcd, 16, 2, LCD_RS, LCD_EN, LCD_DS, LCD_SHCP, LCD_STCP);
    Snake_Init();

    // Tạo queues và semaphore
    buttonQueue = xQueueCreate(10, sizeof(ButtonEvent));
    configASSERT(buttonQueue != NULL);
    
    scoreQueue = xQueueCreate(1, sizeof(int));
    configASSERT(scoreQueue != NULL);
    
    stateMutex = xSemaphoreCreateMutex();
    configASSERT(stateMutex != NULL);

    Button_SetupInterrupts(buttonQueue);
    
    lastResetTime = 0;

    // Tạo các tasks với priority hợp lý
    configASSERT(xTaskCreate(TaskLCD, "LCD", 2048, NULL, 1, NULL) == pdPASS);
    configASSERT(xTaskCreate(TaskButton, "BTN", 2048, NULL, 2, NULL) == pdPASS);
    configASSERT(xTaskCreate(TaskSnake, "SNAKE", 4096, NULL, 1, NULL) == pdPASS);

    int initialScore = Snake_GetScore();
    xQueueOverwrite(scoreQueue, &initialScore);
}

void loop() {
    vTaskDelay(portMAX_DELAY);
}
