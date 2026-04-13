#include <Arduino.h>
#include "quetled.h"
#include "bientoancuc.h"

// ===== FONT A =====
uint8_t fontA[8] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00
};

void setup()
{
    QuetLed_Init(23, 18, 5);

    // Gán dữ liệu chữ A vào buffer
    for(int i = 0; i < 8; i++)
    {
        cot[i] = fontA[i];
    }
}

void loop()
{
    QuetLed_Matrix();
}