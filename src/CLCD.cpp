#include "CLCD.h"

// Hàm gửi dữ liệu nối tiếp sang song song qua 74HC595
static void CLCD_ShiftOut_595(CLCD_Name* LCD, uint8_t Data) {
    digitalWrite(LCD->STCP_PIN, LOW);
    shiftOut(LCD->DS_PIN, LCD->SHCP_PIN, MSBFIRST, Data);
    digitalWrite(LCD->STCP_PIN, HIGH); // Xuất dữ liệu ra Q0-Q7
}

static void CLCD_Write8(CLCD_Name* LCD, uint8_t Data, uint8_t Mode) {
    // Thiết lập RS
    digitalWrite(LCD->RS_PIN, (Mode == CLCD_DATA) ? HIGH : LOW);
    
    // Đẩy dữ liệu ra bus song song của 595
    CLCD_ShiftOut_595(LCD, Data);
    
    // Tạo xung Enable để LCD nhận dữ liệu
    digitalWrite(LCD->EN_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(LCD->EN_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(LCD->EN_PIN, LOW);
    delayMicroseconds(100); // Chờ LCD xử lý
}

void CLCD_595_8BIT_Init(CLCD_Name* LCD, uint8_t Colum, uint8_t Row,
                        uint8_t RS_PIN, uint8_t EN_PIN,
                        uint8_t DS_PIN, uint8_t SHCP_PIN, uint8_t STCP_PIN) 
{
    LCD->MODE = LCD_8BITMODE;
    LCD->COLUMS = Colum;
    LCD->ROWS = Row;
    LCD->RS_PIN = RS_PIN;
    LCD->EN_PIN = EN_PIN;
    LCD->DS_PIN = DS_PIN;
    LCD->SHCP_PIN = SHCP_PIN;
    LCD->STCP_PIN = STCP_PIN;

    // Cấu hình chân Output
    pinMode(RS_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(DS_PIN, OUTPUT);
    pinMode(SHCP_PIN, OUTPUT);
    pinMode(STCP_PIN, OUTPUT);

    delay(50);
    LCD->FUNCTIONSET = LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS;
    LCD->ENTRYMODE = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD->CURSORSHIFT = LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT;

    CLCD_Write8(LCD, LCD->FUNCTIONSET, CLCD_COMMAND);
    delay(5);
    CLCD_Write8(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
    CLCD_Write8(LCD, LCD->ENTRYMODE, CLCD_COMMAND);
    
    CLCD_Clear(LCD);
}

void CLCD_SetCursor(CLCD_Name* LCD, uint8_t Xpos, uint8_t Ypos) {
    uint8_t DRAM_OFFSET[4] = {0x00, 0x40, 0x14, 0x54};
    if (Xpos >= LCD->COLUMS) Xpos = LCD->COLUMS - 1;
    if (Ypos >= LCD->ROWS) Ypos = LCD->ROWS - 1;
    
    CLCD_Write8(LCD, LCD_SETDDRAMADDR | (DRAM_OFFSET[Ypos] + Xpos), CLCD_COMMAND);
}

void CLCD_WriteChar(CLCD_Name* LCD, char character) {
    CLCD_Write8(LCD, character, CLCD_DATA);
}

void CLCD_WriteString(CLCD_Name* LCD, char *String) {
    while (*String) CLCD_WriteChar(LCD, *String++);
}

void CLCD_Clear(CLCD_Name* LCD) {
    CLCD_Write8(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
    delay(2);
}