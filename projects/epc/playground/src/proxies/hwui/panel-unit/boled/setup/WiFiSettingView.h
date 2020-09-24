#pragma once
#include "SetupLabel.h"

class Setting;

class WiFiSettingView : public SetupLabel
{
 private:
  typedef SetupLabel base_type;

 public:
  WiFiSettingView();
  ~WiFiSettingView() override;

 private:
  void onSettingChanged(const Setting *s);
};
