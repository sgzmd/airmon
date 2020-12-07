//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_DATAUPLOADER_H
#define AIRMON_DATAUPLOADER_H


class DataUploader {
public:
    DataUploader() {
      // Nothing to be done here
    };

    virtual void UploadData(float temperature, int co2_level) = 0;
};


#endif //AIRMON_DATAUPLOADER_H
