#pragma once

#include "PlayPanel.h"
#include "UpperRibbon.h"
#include "LowerRibbon.h"
#include "proxies/hwui/HardwareUserInterfaceUnit.h"

class Application;

class BaseUnit : public HardwareUserInterfaceUnit, public sigc::trackable
{
 private:
  typedef HardwareUserInterfaceUnit super;

 public:
  BaseUnit();
  virtual ~BaseUnit();

  void onTimeout();
  void indicateBlockingMainThread(bool onOff);
  void onScreenSaverStateChanged(bool state);

  void init();

  PlayPanel &getPlayPanel();

 private:
  void respectUsageMode(const Setting *s);
  void onBBBBConnected();
  std::shared_ptr<UsageMode> m_stashedUsageMode;

  PlayPanel m_playPanel;
  UpperRibbon m_upperRibbon;
  LowerRibbon m_lowerRibbon;
};
