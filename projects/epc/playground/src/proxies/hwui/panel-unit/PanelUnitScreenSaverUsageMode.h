#pragma once
#include <proxies/hwui/UsageMode.h>

class PanelUnitScreenSaverUsageMode : public UsageMode
{
 public:
  PanelUnitScreenSaverUsageMode();
  bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;
  void setup() override;

 private:
  bool m_downReceived = false;
};