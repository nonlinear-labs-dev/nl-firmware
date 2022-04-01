#include <device-settings/ScreenSaverTimeoutSetting.h>
#include "HardwareUserInterfaceUnit.h"
#include "UsageMode.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/ScreenSaverUsageMode.h>
#include <device-settings/FocusAndModeSetting.h>

HardwareUserInterfaceUnit::HardwareUserInterfaceUnit(Settings& settings)
    : m_settings { settings }
{
  m_settings.getSetting<ScreenSaverTimeoutSetting>()->onScreenSaverStateChanged(
      sigc::mem_fun(this, &HardwareUserInterfaceUnit::onScreenSaverStateChanged));

  m_settings.getSetting<FocusAndModeSetting>()->onChange(
      sigc::mem_fun(this, &HardwareUserInterfaceUnit::onFocusAndModeChanged));
}

HardwareUserInterfaceUnit::~HardwareUserInterfaceUnit()
{
}

void HardwareUserInterfaceUnit::setUsageMode(UsageMode* mode)
{
  if(mode && m_usageMode)
  {
    if(typeid(*mode) == typeid(*m_usageMode.get()))
    {
      delete mode;
      return;
    }
  }

  m_usageMode.reset(mode);
  m_usageMode->setup();
}

void HardwareUserInterfaceUnit::restoreUsageMode(std::shared_ptr<UsageMode> mode)
{
  m_usageMode = mode;

  if(m_usageMode)
    m_usageMode->bruteForceUpdateLeds();
}

std::shared_ptr<UsageMode> HardwareUserInterfaceUnit::getUsageMode() const
{
  if(m_screenSaverActive)
    return getScreenSaverUsageMode();
  return m_usageMode;
}

bool HardwareUserInterfaceUnit::onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state)
{
  if(auto mode = getUsageMode())
    return mode->onButtonPressed(buttonID, modifiers, state);

  return false;
}

void HardwareUserInterfaceUnit::onScreenSaverStateChanged(bool state)
{
  m_screenSaverActive = state;
}

std::shared_ptr<UsageMode> HardwareUserInterfaceUnit::getScreenSaverUsageMode() const
{
  static auto sScreenSaver = std::make_shared<ScreenSaverUsageMode>();
  return sScreenSaver;
}

Settings& HardwareUserInterfaceUnit::getSettings() const
{
  return m_settings;
}

void HardwareUserInterfaceUnit::onFocusAndModeChanged(const Setting* s)
{
  if(auto famSetting = dynamic_cast<const FocusAndModeSetting*>(s))
  {
    auto state = famSetting->getState();
    setupFocusAndMode(state);
  }
}
