#pragma once
#include <device-settings/BooleanSetting.h>

class BenderRampBypass : public BooleanSetting
{
 public:
  BenderRampBypass(Settings& settings);
  virtual ~BenderRampBypass();

 private:
  void sendToLPC() const override;
  bool persistent() const override;
};
