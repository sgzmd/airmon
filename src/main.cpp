#include <Arduino.h>
#include <ThingSpeak.h>
#include <U8x8lib.h>

#include "config_private.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C lcd(
  /* clock=*/ 15, 
  /* data=*/ 4, 
  /* reset=*/ 16);

int i = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin();
}

void loop() {
  
  lcd.setFont(u8x8_font_chroma48medium8_r);
  lcd.drawString(0, i, "Hello world!");


  delay(1000);
}