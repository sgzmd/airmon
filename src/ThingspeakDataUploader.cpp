//
// Created by sgzmd on 07/12/2020.
//

#define PRINT_DEBUG_MESSAGES 1

#include "ThingspeakDataUploader.h"
#include "config_private.h"

#include <HTTPClient.h>

void ThingspeakDataUploader::UploadData(float temperature, int co2_level, float humidity, int tvoc, int pressure,
                                        float gas_resistance) {

  HTTPClient http;

  char *buf = new char[2048];
  sprintf(buf,
          "http://api.thingspeak.com/update?api_key=%s&field1=%f&field2=%d&field3=%f&field4=%d&field5=%d&field6=%f",
          THINGSPEAK_API_KEY,
          temperature,
          co2_level,
          humidity,
          tvoc,
          pressure,
          gas_resistance);

  Serial.printf("Making request %s\n", buf);
  http.begin(buf);
  int code = http.GET();
  if (code > 0) {
    Serial.printf("HTTP code %d, payload %s\n", code, http.getString().c_str());
  } else {
    Serial.printf("HTTP failed, code %d\n", code);
  }

  http.end();
  delete[] buf;
}
