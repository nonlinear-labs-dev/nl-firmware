#pragma once

#include <proxies/hwui/UsageMode.h>
#include <proxies/hwui/ShortVsLongPress.h>

class Application;

class BaseUnitPresetsAndBanksMode : public UsageMode
{
 private:
  typedef UsageMode super;

 public:
  BaseUnitPresetsAndBanksMode();
  void setup() override;

 private:
  virtual void onFuncButtonDown();
  void onLongPress();
  void modeButtonShortPress();
  void modeButtonLongPress();

  ShortVsLongPress m_modeButtonHandler;
  ShortVsLongPress m_funcButtonHandler;
};
