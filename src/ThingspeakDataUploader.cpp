//
// Created by sgzmd on 07/12/2020.
//

#define PRINT_DEBUG_MESSAGES 1

#include <ThingSpeak.h>

#include "ThingspeakDataUploader.h"
#include "config_private.h"

void ThingspeakDataUploader::UploadData(float temperature, int co2_level, float humidity, int tvoc, int pressure) {
  // It appears to be reentrant so it's safe.
  ThingSpeak.begin(_wifiClient);

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, co2_level);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, tvoc);
  ThingSpeak.setField(5, pressure);

  ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
}
