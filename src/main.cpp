#include <Arduino.h>
#include <ThingSpeak.h>
#include <U8x8lib.h>

#include "config_private.h"
#include "LcdDataUploader.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C lcd(
    /* clock=*/ 15,
    /* data=*/ 4,
    /* reset=*/ 16);

LcdDataUploader *lcd_data_uploader;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.setFont(u8x8_font_chroma48medium8_r);
  lcd_data_uploader = new LcdDataUploader(lcd);
}

void loop() {
  lcd_data_uploader->UploadData(21, 115);
  delay(1000);
}