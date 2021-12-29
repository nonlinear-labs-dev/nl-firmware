#pragma once

#include "playground.h"
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/buttons.h>
#include <memory>

class UsageMode;
class Settings;

class HardwareUserInterfaceUnit
{
 private:
 public:
  HardwareUserInterfaceUnit(Settings& settings);
  virtual ~HardwareUserInterfaceUnit();

  HardwareUserInterfaceUnit(const HardwareUserInterfaceUnit& other) = delete;
  HardwareUserInterfaceUnit& operator=(const HardwareUserInterfaceUnit&) = delete;

  virtual bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state);

  void setUsageMode(UsageMode* mode);
  void restoreUsageMode(std::shared_ptr<UsageMode> mode);
  [[nodiscard]] std::shared_ptr<UsageMode> getUsageMode() const;
  [[nodiscard]] std::shared_ptr<UsageMode> getScreenSaverUsageMode() const;

 protected:
  [[nodiscard]] Settings& getSettings() const;
  virtual void setupFocusAndMode(FocusAndMode focusAndMode) = 0;

 private:
  void onScreenSaverStateChanged(bool state);
  void onFocusAndModeChanged(const Setting* s);
  bool m_screenSaverActive = false;
  std::shared_ptr<UsageMode> m_usageMode;
  Settings& m_settings;
};
