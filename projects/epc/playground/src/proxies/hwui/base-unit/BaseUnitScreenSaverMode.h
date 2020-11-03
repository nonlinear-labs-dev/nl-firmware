#pragma once
#include <proxies/hwui/UsageMode.h>

class BaseUnitScreenSaverMode : public UsageMode
{
 public:
  void setup() override;
  bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;

 private:
  bool m_downReceived = false;
};
