#include <Arduino.h>

#include "config_private.h"
#include "LcdAndThingspeak.h"

LcdAndThingspeak *uploader;

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
}

void loop() {
  uploader->UploadData(21, 115);
  delay(10000);
}