#include "ScreenSaverUsageMode.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>

ScreenSaverUsageMode::ScreenSaverUsageMode()
    : UsageMode()
{
}

void ScreenSaverUsageMode::setup()
{
}

bool ScreenSaverUsageMode::onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state)
{
  if(state)
  {
    m_downReceived = true;
  }
  else if(m_downReceived)
  {
    Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->endAndReschedule();
  }

  return true;
}
