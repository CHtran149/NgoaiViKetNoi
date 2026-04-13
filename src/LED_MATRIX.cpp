#include "LED_Matrix.h"

// ===== INIT =====
void LEDMATRIX_Init(LEDMATRIX_Name* matrix,
                    uint8_t DS, uint8_t SHCP, uint8_t STCP,
                    uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                    uint8_t r4, uint8_t r5, uint8_t r6, uint8_t r7) 
{
    matrix->DS_PIN = DS;
    matrix->SHCP_PIN = SHCP;
    matrix->STCP_PIN = STCP;

    matrix->rowPins[0] = r0;
    matrix->rowPins[1] = r1;
    matrix->rowPins[2] = r2;
    matrix->rowPins[3] = r3;
    matrix->rowPins[4] = r4;
    matrix->rowPins[5] = r5;
    matrix->rowPins[6] = r6;
    matrix->rowPins[7] = r7;

    matrix->scanDelay = 300; // 300us → mượt, không nhấp nháy

    pinMode(matrix->DS_PIN, OUTPUT);
    pinMode(matrix->SHCP_PIN, OUTPUT);
    pinMode(matrix->STCP_PIN, OUTPUT);

    for (int i = 0; i < 8; i++) {
        pinMode(matrix->rowPins[i], OUTPUT);
        digitalWrite(matrix->rowPins[i], HIGH); // HIGH = tắt (Cathode hàng)
    }
}

// ===== SHIFT DATA TO 74HC595 =====
static void LEDMATRIX_ShiftOut(LEDMATRIX_Name* matrix, uint8_t data) {
    digitalWrite(matrix->STCP_PIN, LOW);
    shiftOut(matrix->DS_PIN, matrix->SHCP_PIN, MSBFIRST, data);
    digitalWrite(matrix->STCP_PIN, HIGH);
}

// ===== CLEAR =====
void LEDMATRIX_Clear(LEDMATRIX_Name* matrix) {
    // tắt toàn bộ hàng trước
    for (int i = 0; i < 8; i++) {
        digitalWrite(matrix->rowPins[i], HIGH);
    }

    // tắt toàn bộ cột
    LEDMATRIX_ShiftOut(matrix, 0x00);
}

// ===== SCAN DISPLAY =====
void LEDMATRIX_Scan(LEDMATRIX_Name* matrix, uint8_t *buffer) {
    for (uint8_t i = 0; i < 8; i++) {

        // 1. Tắt toàn bộ hàng (chống ghosting)
        for (uint8_t j = 0; j < 8; j++) {
            digitalWrite(matrix->rowPins[j], HIGH);
        }

        // 2. Gửi dữ liệu cột
        LEDMATRIX_ShiftOut(matrix, buffer[i]);
        // Nếu bị ngược → dùng:
        // LEDMATRIX_ShiftOut(matrix, ~buffer[i]);

        // 3. Bật hàng hiện tại
        digitalWrite(matrix->rowPins[i], LOW);

        // 4. Delay quét
        delayMicroseconds(matrix->scanDelay);
    }
}