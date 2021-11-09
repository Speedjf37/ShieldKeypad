


// Gestion de clavier 
// LIB MD_UISwitch

#include <MD_UISwitch.h>

const uint8_t ANALOG_SWITCH_PIN = A0;       // switches connected to this pin



#define btnRIGHT  5
#define btnUP     4
#define btnDOWN   3
#define btnLEFT   2
#define btnSELECT 1
#define btnNONE   0


// These key values work for most LCD shields
MD_UISwitch_Analog::uiAnalogKeys_t kt[] =
{
  {  10, 10, btnRIGHT },  // Right
  { 100, 15, btnUP },  // Up
  { 255, 15, btnDOWN },  // Down
  { 410, 15, btnLEFT },  // Left
  { 640, 15, btnSELECT },  // Select
};

/*
// These key values work for most LCD shields
MD_UISwitch_Analog::uiAnalogKeys_t kt[] =
{
  {  10, 10, 'R' },  // Right
  { 100, 15, 'U' },  // Up
  { 255, 15, 'D' },  // Down
  { 410, 15, 'L' },  // Left
  { 640, 15, 'S' },  // Select
};
*/

MD_UISwitch_Analog S(ANALOG_SWITCH_PIN, kt, ARRAY_SIZE(kt));

void SetupClavier(void)
{
  S.begin();
  //S.enableDoublePress(false);
  S.enableLongPress(true);
  S.enableRepeat(true);
  //S.enableRepeat(false);
  S.enableRepeatResult(false);
}

void TestClavier(void)
{
  MD_UISwitch::keyResult_t k = S.read();

  switch(k)
  {
    case MD_UISwitch::KEY_NULL:      /* Serial.print("KEY_NULL"); */  break;
    case MD_UISwitch::KEY_UP:        Serial.print("\nKEY_UP ");     break;
    case MD_UISwitch::KEY_DOWN:      Serial.print("\nKEY_DOWN ");   break;
    case MD_UISwitch::KEY_PRESS:     Serial.print("\nKEY_PRESS ");  break;
    case MD_UISwitch::KEY_DPRESS:    Serial.print("\nKEY_DOUBLE "); break;
    case MD_UISwitch::KEY_LONGPRESS: Serial.print("\nKEY_LONG "); break;
    case MD_UISwitch::KEY_RPTPRESS:  Serial.print("\nKEY_REPEAT "); break;
    default:                         Serial.print("\nKEY_UNKNWN "); break;
  }
  if (k != MD_UISwitch::KEY_NULL)
  {
    if (S.getKey() >= ' ')
    {
      Serial.print((char)S.getKey());
      Serial.print(" ");
    }
    Serial.print("[0x");
    Serial.print(S.getKey(), HEX);
    Serial.print("]");
  }
}
