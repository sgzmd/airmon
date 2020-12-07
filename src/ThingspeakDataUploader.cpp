//
// Created by sgzmd on 07/12/2020.
//

#define PRINT_DEBUG_MESSAGES 1
#include <ThingSpeak.h>

#include "ThingspeakDataUploader.h"
#include "config_private.h"

void ThingspeakDataUploader::UploadData(float temperature, int co2_level) {
  // It appears to be reentrant so it's safe.
  ThingSpeak.begin(_wifiClient);

  ThingSpeak.writeField(THINGSPEAK_CHANNEL_ID, 1, temperature, THINGSPEAK_API_KEY);
  ThingSpeak.writeField(THINGSPEAK_CHANNEL_ID, 2, co2_level, THINGSPEAK_API_KEY);
}
