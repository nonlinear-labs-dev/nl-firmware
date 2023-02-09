#include "ScreenSaverConnections.h"
#include <proxies/hwui/HWUI.h>
#include "proxies/playcontroller/PlaycontrollerProxy.h"
#include "presets/EditBuffer.h"
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <device-settings/Settings.h>

ScreenSaverConnections::ScreenSaverConnections(ScreenSaverTimeoutSetting& setting)
    : m_setting { setting }
{
}

void ScreenSaverConnections::connectRescheduleToSources(HWUI& hwui, EditBuffer& eb, PlaycontrollerProxy& pcp,
                                                        Settings& settings)
{
  auto& boled = hwui.getPanelUnit().getEditPanel().getBoled();

  auto reschedule = sigc::mem_fun(m_setting, &ScreenSaverTimeoutSetting::endAndReschedule);
  auto onLayoutInstalled = sigc::mem_fun(m_setting, &ScreenSaverTimeoutSetting::onLayoutInstalled);

  eb.onChange(reschedule, false);
  eb.onSelectionChanged(sigc::hide(sigc::hide(reschedule)), std::nullopt);
  pcp.onLastKeyChanged(reschedule);
  settings.onSettingsChanged(reschedule);
  hwui.onButtonPressed(sigc::hide(sigc::hide(reschedule)));

  boled.onLayoutInstalled(onLayoutInstalled);
}
