#pragma once
#include <proxies/hwui/UsageMode.h>

class ScreenSaverUsageMode : public UsageMode
{
 public:
  ScreenSaverUsageMode();
  bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;
  void setup() override;

 private:
  bool m_downReceived = false;
};