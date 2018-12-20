#pragma once
#include "BooleanSetting.h"

class WifiSetting : public BooleanSetting
{
 public:
  typedef BooleanSetting super;
  explicit WifiSetting(Settings& settings);

  ~WifiSetting() override;

  bool set(tEnum m) override;
  static bool pollAccessPointRunning();
};