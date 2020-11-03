#include "PanelUnitScreenSaverUsageMode.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>

PanelUnitScreenSaverUsageMode::PanelUnitScreenSaverUsageMode()
    : UsageMode()
{
}

void PanelUnitScreenSaverUsageMode::setup()
{
}

bool PanelUnitScreenSaverUsageMode::onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state)
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
