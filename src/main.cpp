#include <Arduino.h>
#include "CLCD.h"

// Khai báo đối tượng LCD
CLCD_Name LCD1;

// --- Cấu hình chân kết nối ---
// Chân điều khiển trực tiếp
#define LCD_RS_PIN    4
#define LCD_EN_PIN    2

// Chân giao tiếp với IC 74HC595
#define SIPO_DS       23  // Serial Data (Chân 14 của 595)
#define SIPO_SHCP     18  // Shift Clock (Chân 11 của 595)
#define SIPO_STCP     5   // Latch Clock (Chân 12 của 595)

void setup() {
  // 1. Khởi tạo LCD ở chế độ 8-bit qua IC 74HC595
  // Thông số: &đối tượng, số cột, số hàng, RS, EN, DS, SHCP, STCP
  CLCD_595_8BIT_Init(&LCD1, 16, 2, 
                     LCD_RS_PIN, LCD_EN_PIN, 
                     SIPO_DS, SIPO_SHCP, SIPO_STCP);

  // 2. Hiển thị nội dung hàng 1
  CLCD_SetCursor(&LCD1, 0, 0); // Cột 0, Hàng 0
  CLCD_WriteString(&LCD1, "BTL: CHIP 595");

  // 3. Hiển thị nội dung hàng 2
  CLCD_SetCursor(&LCD1, 0, 1); // Cột 0, Hàng 1
  CLCD_WriteString(&LCD1, "ESP32 8-BIT MODE");
  
  delay(2000); // Chờ 2 giây để quan sát
}

void loop() {
  // Hiệu ứng nhấp nháy đơn giản để kiểm tra tính ổn định
  CLCD_SetCursor(&LCD1, 0, 1);
  CLCD_WriteString(&LCD1, "   READY TO GO!   ");
  delay(1000);
  
  CLCD_SetCursor(&LCD1, 0, 1);
  CLCD_WriteString(&LCD1, " ESP32 IS ACTIVE ");
  delay(1000);
}