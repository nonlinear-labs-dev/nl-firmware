#pragma once

#include "playground.h"
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/buttons.h>
#include <memory>

class UsageMode;

class HardwareUserInterfaceUnit
{
 private:
 public:
  HardwareUserInterfaceUnit();
  virtual ~HardwareUserInterfaceUnit();

  HardwareUserInterfaceUnit(const HardwareUserInterfaceUnit& other) = delete;
  HardwareUserInterfaceUnit& operator=(const HardwareUserInterfaceUnit&) = delete;

  virtual void setupFocusAndMode(FocusAndMode focusAndMode);
  virtual bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state);

  void setUsageMode(UsageMode* mode);
  void restoreUsageMode(std::shared_ptr<UsageMode> mode);
  std::shared_ptr<UsageMode> getUsageMode();
  [[nodiscard]] std::shared_ptr<const UsageMode> getUsageMode() const;

 private:
  std::shared_ptr<UsageMode> m_usageMode;
};
