
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const byte rowsLCD      = 2;     // Nb rows  LCD
const byte columnsLCD   = 16;    // Nb colum LCD
const byte iArrow[8]    = { B00000, B00100, B00110, B11111, B00110, B00100, B00000, B00000 };
const byte ARROW        = 0;
byte customBackslash[8] = {  0b00000,  0b10000,  0b01000,  0b00100,  0b00010,  0b00001,  0b00000,  0b00000};


/*-----( Declare Variables )-----*/
int lcd_key       = 0;
int adc_key_in    = 0;
int adc_key_prev  = 0;


SetupLcd()
{
    lcd.begin(columnsLCD, rowsLCD);
    lcd.createChar(ARROW, iArrow);
    lcd.createChar(7, customBackslash);
}
