#include <Arduino.h>

#include "config_private.h"
#include "LcdAndThingspeak.h"
#include "DataProvider.h"
#include "FakeDataProvider.h"

constexpr unsigned long uS_TO_S_FACTOR = 1000000;  /* Conversion factor for micro seconds to seconds */
constexpr unsigned short TIME_TO_SLEEP = 3;        /* Time ESP32 will go to sleep (in seconds) */


void setup() {
  // Chip goes to sleep and will reboot at the end of setup.
  loop();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  Serial.begin(9600);

  LcdAndThingspeak *uploader = nullptr;
  DataProvider* provider = nullptr;

  WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  WiFiClient client;
  uploader = new LcdAndThingspeak(client);
  uploader->SetIpAddress(WiFi.localIP().toString());

  provider = new FakeDataProvider();


  uploader->UploadData(provider->GetTemperature(),
                       provider->GetCO2Level());

  delay(1000);

  delete provider;
  delete uploader;

  Serial.end();
}