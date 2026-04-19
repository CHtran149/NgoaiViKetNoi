#include "quetled.h"
#include "bientoancuc.h"

// ===== CHÂN =====
static uint8_t DS_PIN;
static uint8_t SHCP_PIN;
static uint8_t STCP_PIN;

// ===== CẤU HÌNH ĐỘ SÁNG =====
// Tổng thời gian quét 1 hàng (chuẩn là 300us)
#define SCAN_DELAY_US 300 

// Thời gian bật LED thực tế (0 đến 300). 
// CÀNG NHỎ -> CÀNG TỐI -> CÀNG TỐN ÍT ĐIỆN.
#define BRIGHTNESS_US 50  

// ===== INIT =====
void QuetLed_Init(uint8_t DS, uint8_t SHCP, uint8_t STCP)
{
    DS_PIN = DS;
    SHCP_PIN = SHCP;
    STCP_PIN = STCP;

    pinMode(DS_PIN, OUTPUT);
    pinMode(SHCP_PIN, OUTPUT);
    pinMode(STCP_PIN, OUTPUT);
}

// ===== GỬI 2 BYTE =====
static void Shift595(uint8_t rowData, uint8_t colData)
{
    digitalWrite(STCP_PIN, LOW);

    // IC2 (cột)
    shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, colData);

    // IC1 (hàng)
    shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, rowData);

    digitalWrite(STCP_PIN, HIGH);
}

// ===== QUÉT MATRIX =====
void QuetLed_Matrix()
{
    for(int i = 0; i < 8; i++)
    {
        uint8_t x = cot[i];   // dữ liệu cột
        uint8_t y = hang[i];  // chọn hàng

        // 1. Bật sáng LED ở hàng hiện tại
        Shift595(y, x);
        delayMicroseconds(BRIGHTNESS_US); // Chỉ sáng một lúc ngắn

        // 2. Tắt LED ngay lập tức (Cắt dòng điện)
        Shift595(0xFF, 0x00);
        
        // 3. Nghỉ ngơi trong phần thời gian còn lại của chu kỳ
        delayMicroseconds(SCAN_DELAY_US - BRIGHTNESS_US); 
    }
}