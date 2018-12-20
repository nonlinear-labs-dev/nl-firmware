#pragma once

#include "BaseUnitPresetsAndBanksMode.h"

class BaseUnitPresetsMode : public BaseUnitPresetsAndBanksMode
{
 private:
  typedef BaseUnitPresetsAndBanksMode super;

 public:
  void setup() override;

 private:
  void onFuncButtonDown() override;
};
