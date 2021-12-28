#pragma once

#include <device-settings/FocusAndModeSetting.h>
#include "PlayPanel.h"
#include "UpperRibbon.h"
#include "LowerRibbon.h"
#include "proxies/hwui/HardwareUserInterfaceUnit.h"

class Application;
class Settings;

class BaseUnit : public HardwareUserInterfaceUnit, public sigc::trackable
{
 private:
  typedef HardwareUserInterfaceUnit super;

 public:
  BaseUnit(Settings& settings);
  ~BaseUnit() override;

  void onTimeout();
  void indicateBlockingMainThread(bool onOff);

  void init();

  PlayPanel &getPlayPanel();
  void setupFocusAndMode(FocusAndMode focusAndMode) override;

 private:
  void respectUsageMode(const Setting *s);
  void onBBBBConnected();

  PlayPanel m_playPanel;
  UpperRibbon m_upperRibbon;
  LowerRibbon m_lowerRibbon;
};
