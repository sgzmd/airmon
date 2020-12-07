//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_LCDDATAUPLOADER_H
#define AIRMON_LCDDATAUPLOADER_H

#include <U8x8lib.h>
#include "DataUploader.h"

typedef U8X8_SSD1306_128X64_NONAME_SW_I2C I2CLcd;

class LcdDataUploader : DataUploader {
private:
    I2CLcd _lcd;
public:
    LcdDataUploader(I2CLcd& lcd) : _lcd(lcd) {}
    void UploadData(float temperature, int co2_level) override;
};


#endif //AIRMON_LCDDATAUPLOADER_H