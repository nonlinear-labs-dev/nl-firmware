#include "ScreenSaverUsageMode.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/TwoStateLED.h>

ScreenSaverUsageMode::ScreenSaverUsageMode()
    : UsageMode()
{
}

void ScreenSaverUsageMode::setup()
{
  auto& panelUnit = Application::get().getHWUI()->getPanelUnit();
  for(auto& l : panelUnit.getLeds())
  {
    l->setState(TwoStateLED::LedState::OFF);
  }
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
