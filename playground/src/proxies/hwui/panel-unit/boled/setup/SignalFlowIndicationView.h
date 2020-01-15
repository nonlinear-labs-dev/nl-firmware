#pragma once

#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>

class Setting;

class SignalFlowIndicationView : public SetupLabel
{
 public:
  SignalFlowIndicationView();
  void onSettingChanged(const Setting *s);
};
