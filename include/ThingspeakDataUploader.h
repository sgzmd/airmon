//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_THINGSPEAKDATAUPLOADER_H
#define AIRMON_THINGSPEAKDATAUPLOADER_H

#include <WiFi.h>

class ThingspeakDataUploader {
private:
    WiFiClient _wifiClient;
public:
    ThingspeakDataUploader() {}
    virtual ~ThingspeakDataUploader() {}
    void UploadData(float temperature, int co2_level, float humidity, int tvoc, int pressure);
};


#endif //AIRMON_THINGSPEAKDATAUPLOADER_H
