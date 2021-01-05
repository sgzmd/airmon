#include <Arduino.h>
#include <ccs811.h>
#include <Adafruit_BME680.h>
#include <U8x8lib.h>
#include <WString.h>
#include <WiFi.h>
#include <Wire.h>

#include <bsec.h>

#include "config_private.h"
#include "ThingspeakDataUploader.h"
#include "bsec_helpers.h"

typedef U8X8_SSD1306_128X64_NONAME_SW_I2C I2CLcd;

// BME chip select pin
constexpr unsigned int BME_CS = 5;

CCS811 *ccs811 = nullptr;
I2CLcd *lcd = nullptr;
ThingspeakDataUploader* uploader = nullptr;
Bsec* bsec = nullptr;
SPIClass* spi = nullptr;

String localIP = "";

template<class T>
void draw_data(T data, const char *format, int line);

void connect_wifi() {
  WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  localIP = WiFi.localIP().toString();
}

void setup() {
  Serial.begin(115200);

  connect_wifi();

  Wire.begin();

  lcd = new I2CLcd(
      /* clock=*/ 15,
      /* data=*/ 4,
      /* reset=*/ 16);
  lcd->setFont(u8x8_font_chroma48medium8_r);
  lcd->begin();

  ccs811 = new CCS811();

  // Attempt to initialize CCS811
  while (!ccs811->begin()) {
    Serial.println("*** Unable to find CCS811. Waiting 3 seconds.");
    delay(3000);
  }

  // Start measuring with CCS811
  if (!ccs811->start(CCS811_MODE_1SEC)) {
    Serial.println("*** Failed to start CCS811 measurement. This program will now stop.");
    lcd->drawString(0, 0, "CCS811 failure");
    while (true) {}
  }

  // Initialising BME680 via Bsec library
  spi = new SPIClass();
  spi->begin();
  bsec = new Bsec();
  bsec->begin(BME_CS, *spi);
  checkIaqSensorStatus(*bsec);

  uploader = new ThingspeakDataUploader();
}

void loop() {
  uint16_t eco2, etvoc, errstat, raw;
  ccs811->read(&eco2, &etvoc, &errstat, &raw);
  Serial.println(ccs811->errstat_str(errstat));

  if (eco2 > 20000) {
    // Not a sane value, can produce at startup.
    return;
  }

  draw_data(eco2, "eCO2: %u", 4);
  draw_data(etvoc, "eTVOC: %u", 5);

  if (bsec->run()) {
    Serial.printf("IAQ: %f, accuracy: %f\n", bsec->iaq, bsec->iaqAccuracy);
  }

  // draw_data(bme->temperature, "Temp:\t%.1f", 0);
  // draw_data(bme->humidity, "Humid:\t%.1f", 1);
  // draw_data(bme->pressure / 100, "Press:\t%d hPa", 2);
  // draw_data(bme->gas_resistance / 1000, "Gas:\t%d", 3);

  if (!localIP.isEmpty()) {
    draw_data(localIP.c_str(), "%s", 6);
  }

  // uploader->UploadData(bme->temperature,
  //                      eco2,
  //                      bme->humidity,
  //                      etvoc,
  //                      bme->pressure / 100,
  //                      bme->gas_resistance / 1000.0);

  delay(5000);
}

template<class T>
void draw_data(T data, const char *format, int line) {
  lcd->clearLine(line);

  char str[64];
  memset(str, 0, sizeof(str));
  sprintf(str, format, data);
  lcd->drawString(0, line, str);

  Serial.println(str);
}
