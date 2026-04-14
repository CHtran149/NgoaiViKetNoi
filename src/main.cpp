#include <Arduino.h>
#include "quetled.h"
#include "bientoancuc.h"

// ===== FONT A =====


uint8_t fontT[8] = {
  0x7E,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x00
};

uint8_t fontI[8] = {
  0x7E,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x7E,
  0x00
};

void HienThiChu(uint8_t *font, int time_ms)
{
    unsigned long t = millis();

    while(millis() - t < time_ms)
    {
        for(int i = 0; i < 8; i++)
        {
            cot[i] = font[i];
        }

        QuetLed_Matrix();
    }
}

void setup()
{
    QuetLed_Init(23, 18, 5);

    // Gán dữ liệu chữ A vào buffer
}

void loop()
{
    HienThiChu(fontT, 1000);
}