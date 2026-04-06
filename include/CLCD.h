#ifndef __CLCD_H
#define __CLCD_H

#include <Arduino.h>

// constant
#define CLCD_COMMAND            0x00
#define CLCD_DATA               0x01

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00
#define LCD_CURSOROFF           0x00

// flags for display/cursor shift
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_5x8DOTS             0x00

typedef struct {
    uint8_t MODE;
    uint8_t COLUMS; 
    uint8_t ROWS; 
    
    // Chân điều khiển trực tiếp từ ESP32
    uint8_t RS_PIN;
    uint8_t EN_PIN;
    
    // Chân giao tiếp với 74HC595
    uint8_t DS_PIN;    // Serial Data
    uint8_t SHCP_PIN;  // Shift Clock
    uint8_t STCP_PIN;  // Latch Clock

    uint8_t ENTRYMODE;
    uint8_t DISPLAYCTRL;
    uint8_t CURSORSHIFT;
    uint8_t FUNCTIONSET;
} CLCD_Name;

// Hàm khởi tạo riêng cho phương án dùng 74HC595
void CLCD_595_8BIT_Init(CLCD_Name* LCD, uint8_t Colum, uint8_t Row,
                        uint8_t RS_PIN, uint8_t EN_PIN,
                        uint8_t DS_PIN, uint8_t SHCP_PIN, uint8_t STCP_PIN);

void CLCD_SetCursor(CLCD_Name* LCD, uint8_t Xpos, uint8_t YPos);
void CLCD_WriteChar(CLCD_Name* LCD, char character);
void CLCD_WriteString(CLCD_Name* LCD, char *String);
void CLCD_Clear(CLCD_Name* LCD);
void CLCD_ReturnHome(CLCD_Name* LCD);

#endif