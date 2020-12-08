//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_DATAPROVIDER_H
#define AIRMON_DATAPROVIDER_H

class DataProvider {
public:
  virtual int GetCO2Level() = 0;
  virtual float GetTemperature() = 0;
  virtual ~DataProvider() {};
};


#endif //AIRMON_DATAPROVIDER_H
