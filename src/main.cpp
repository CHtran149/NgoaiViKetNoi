#include <Arduino.h>
#include "bientoancuc.h"
#include "button.h"
#include "quetled.h"

#define SIPO_DS   23
#define SIPO_SHCP 18
#define SIPO_STCP 5

int currentX = 4;
int currentY = 4;

const uint8_t MAU_COT[8]  = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
const uint8_t MAU_HANG[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

void CapNhatToaDo(int x, int y) {
    // Tắt toàn bộ dữ liệu quét trước khi nạp điểm mới
    for (int i = 0; i < 8; i++) {
        hang[i] = 0xFF; // Tắt cực âm (GND)
        cot[i]  = 0x00; // Tắt cực dương (VCC)
    }

    // Nạp điểm sáng duy nhất vào đúng mảng quetled đang dùng
    if (y >= 0 && y < 8) {
        hang[y] = MAU_HANG[y];
        cot[y]  = MAU_COT[x];
    }
}

void setup() {
    Button_Init();
    QuetLed_Init(SIPO_DS, SIPO_SHCP, SIPO_STCP);
    CapNhatToaDo(currentX, currentY);
}

void loop() {
    static Direction lastInput = DIR_NONE; 
    
    // Lấy giá trị tức thời từ nút bấm
    Direction currentInput = Button_ReadDirection(DIR_NONE);

    // CHỐNG NHẢY LED: Chỉ thực hiện khi có sự thay đổi trạng thái nút
    if (currentInput != lastInput) {
        if (currentInput != DIR_NONE) {
            switch (currentInput) {
                case DIR_UP:    if (currentY > 0) currentY--; break; 
                case DIR_DOWN:  if (currentY < 7) currentY++; break; 
                
                // ĐÃ ĐẢO LẠI TRÁI/PHẢI THEO PHẢN HỒI CỦA BẠN
                case DIR_LEFT:  if (currentX < 7) currentX++; break; // Đổi từ -- thành ++
                case DIR_RIGHT: if (currentX > 0) currentX--; break; // Đổi từ ++ thành --
                
                default: break;
            }
            CapNhatToaDo(currentX, currentY);
        }
        lastInput = currentInput; 
    }

    if (Button_IsReset()) {
        currentX = 4; currentY = 4;
        lastInput = DIR_NONE;
        CapNhatToaDo(currentX, currentY);
    }

    // QUÉT LED
    QuetLed_Matrix();
}