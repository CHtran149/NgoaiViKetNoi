#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H

#include <Arduino.h>

typedef struct {
    // 74HC595 điều khiển Cột (VCC)
    uint8_t DS_PIN;
    uint8_t SHCP_PIN;
    uint8_t STCP_PIN;

    // 8 chân điều khiển Hàng (GND) nối trực tiếp ESP32
    uint8_t rowPins[8];

    // Tốc độ quét (mặc định 500-1000us)
    uint16_t scanDelay; 
} LEDMATRIX_Name;

// Khởi tạo
void LEDMATRIX_Init(LEDMATRIX_Name* matrix,
                    uint8_t DS, uint8_t SHCP, uint8_t STCP,
                    uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                    uint8_t r4, uint8_t r5, uint8_t r6, uint8_t r7);

// Quét LED (Gọi liên tục trong loop)
void LEDMATRIX_Scan(LEDMATRIX_Name* matrix, uint8_t *buffer);

// Xóa màn hình
void LEDMATRIX_Clear(LEDMATRIX_Name* matrix);

#endif