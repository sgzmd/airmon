//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_LCDANDTHINGSPEAK_H
#define AIRMON_LCDANDTHINGSPEAK_H

#include <WiFi.h>
#include <WString.h>

#include "LcdDataUploader.h"
#include "ThingspeakDataUploader.h"

class LcdAndThingspeak : DataUploader {
private:
    LcdDataUploader *_lcd_uploader;
    ThingspeakDataUploader *_thingspeak_uploader;
public:
    LcdAndThingspeak(WiFiClient &wifi) : _lcd_uploader(new LcdDataUploader()),
                                         _thingspeak_uploader(new ThingspeakDataUploader(wifi)) {}

    void UploadData(float temperature, int co2_level) override {
      _thingspeak_uploader->UploadData(temperature, co2_level);
      _lcd_uploader->UploadData(temperature, co2_level);
    }

    virtual ~LcdAndThingspeak() {
      delete _lcd_uploader;
      delete _thingspeak_uploader;
    }

    void SetIpAddress(const String &ip) {
      _lcd_uploader->SetIpAddress(ip);
    }
};


#endif //AIRMON_LCDANDTHINGSPEAK_H
