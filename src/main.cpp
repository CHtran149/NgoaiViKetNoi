#include <Arduino.h>
#include "LED_Matrix.h"

LEDMATRIX_Name Matrix1;

// Giữ nguyên cấu hình chân như bài trước
#define MATRIX_DS     23 
#define MATRIX_SHCP   18
#define MATRIX_STCP   5

#define ROW1 13 // 9
#define ROW2 19 // 14 
#define ROW3 14 //8
#define ROW4 27 // 12
#define ROW5 26 // 1
#define ROW6 25 // 7 
#define ROW7 33 // 2
#define ROW8 32 // 5

// Bộ đệm hiển thị (mặc định tắt hết)
uint8_t displayBuffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  LEDMATRIX_Init(&Matrix1, 
                 MATRIX_DS, MATRIX_SHCP, MATRIX_STCP,
                 ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8);
}

void loop() {
  static uint8_t currentRow = 0;
  static uint8_t currentCol = 0;
  static uint32_t lastMoveTime = 0;
  uint32_t moveInterval = 200; // Tốc độ nhảy LED (200ms mỗi bóng)

  // 1. Logic cập nhật vị trí LED sau mỗi khoảng thời gian
  if (millis() - lastMoveTime > moveInterval) {
    lastMoveTime = millis();

    // Xóa bóng cũ (tắt hết buffer)
    for(int i=0; i<8; i++) displayBuffer[i] = 0;

    // Bật bóng mới tại vị trí (currentRow, currentCol)
    // MSBFIRST thì bit 7 là cột 1, bit 0 là cột 8 (hoặc ngược lại tùy cách nối dây)
    displayBuffer[currentRow] = (1 << currentCol); 

    // Tăng vị trí cột
    currentCol++;
    if (currentCol >= 8) {
      currentCol = 0;
      currentRow++; // Hết cột thì xuống hàng
    }
    
    if (currentRow >= 8) {
      currentRow = 0; // Hết hàng thì quay lại từ đầu (a11)
    }
  }

  // 2. Luôn luôn gọi Scan để duy trì việc hiển thị
  LEDMATRIX_Scan(&Matrix1, displayBuffer);
}