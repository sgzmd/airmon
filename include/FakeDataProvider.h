//
// Created by sgzmd on 07/12/2020.
//

#ifndef AIRMON_FAKEDATAPROVIDER_H
#define AIRMON_FAKEDATAPROVIDER_H

#include "DataProvider.h"

class FakeDataProvider : public DataProvider {
    int GetCO2Level() override;

    float GetTemperature() override;

    virtual ~FakeDataProvider() {}
};


#endif //AIRMON_FAKEDATAPROVIDER_H
