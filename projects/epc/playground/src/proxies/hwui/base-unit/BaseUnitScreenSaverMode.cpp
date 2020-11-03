#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <device-settings/Settings.h>
#include <Application.h>
#include "BaseUnitScreenSaverMode.h"

void BaseUnitScreenSaverMode::setup()
{
}

bool BaseUnitScreenSaverMode::onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state)
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
