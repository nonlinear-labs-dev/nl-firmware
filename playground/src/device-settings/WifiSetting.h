#pragma once
#include "BooleanSetting.h"

class WifiSetting : public BooleanSetting
{
 public:
  explicit WifiSetting(Settings& settings);

  ~WifiSetting() override;

  bool set(Glib::ustring value);
  static bool pollAccessPointRunning();
};