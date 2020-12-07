#include <Arduino.h>

#include "config_private.h"
#include "LcdAndThingspeak.h"
#include "DataProvider.h"
#include "FakeDataProvider.h"

LcdAndThingspeak *uploader = nullptr;
DataProvider* provider = nullptr;

void setup() {
  Serial.begin(9600);

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
}

void loop() {
  uploader->UploadData(provider->GetTemperature(),
                       provider->GetCO2Level());
  delay(10000);
}