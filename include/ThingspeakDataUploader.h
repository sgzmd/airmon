//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_THINGSPEAKDATAUPLOADER_H
#define AIRMON_THINGSPEAKDATAUPLOADER_H

#include "DataUploader.h"

#include <WiFi.h>

class ThingspeakDataUploader : public DataUploader {
private:
    WiFiClient _wifiClient;
public:
    ThingspeakDataUploader(WiFiClient &client) : _wifiClient(client) {

    }

    void UploadData(float temperature, int co2_level) override;
};


#endif //AIRMON_THINGSPEAKDATAUPLOADER_H
