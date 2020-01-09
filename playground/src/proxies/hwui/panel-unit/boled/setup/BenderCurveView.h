#pragma once

#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>

class Setting;

class BenderCurveView : public SetupLabel
{
 private:
  typedef SetupLabel base_type;

 public:
  BenderCurveView();
  virtual ~BenderCurveView();

 private:
  void onSettingChanged(const Setting *s);
};
