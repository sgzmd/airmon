//
// Created by sgzmd on 07/12/2020.
//

#include "FakeDataProvider.h"

#include <cstdlib>

int FakeDataProvider::GetCO2Level() {
  return rand() % 100 + 200;
}

float FakeDataProvider::GetTemperature() {
  return rand() % 20 + 10;
}
