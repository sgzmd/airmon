//
// Created by sgzmd on 07/12/2020.
//

#include <stdlib.h>
#include "../include/LcdDataUploader.h"

void LcdDataUploader::UploadData(float temperature, int co2_level) {
  char str[64];
  sprintf(str, "Temp:\t%.1f", temperature);
  _lcd.drawString(0, 0, str);
  sprintf(str, "CO2:\t\t%d", co2_level);
  _lcd.drawString(0, 1, str);

  if (!_ip.isEmpty()) {
    sprintf(str, "%s", _ip.c_str());
    _lcd.drawString(0, 6, str);
  }
}
