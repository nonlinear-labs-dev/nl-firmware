#include "SplashScreenUseCases.h"
#include <device-settings/Settings.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <Application.h>

SplashScreenUseCases::SplashScreenUseCases(HWUI &hwui, Settings &settings)
    : m_hwui { hwui }
    , m_settings { settings }
{
}

void SplashScreenUseCases::startSplashScreen()
{
  auto screensaver = m_settings.getSetting<ScreenSaverTimeoutSetting>();
  auto &boled = m_hwui.getPanelUnit().getEditPanel().getBoled();
  screensaver->endAndReschedule();
  boled.setOverlay(new SplashLayout());
}

void SplashScreenUseCases::addSplashScreenMessage(const Glib::ustring &string)
{
  auto screensaver = m_settings.getSetting<ScreenSaverTimeoutSetting>();
  screensaver->endAndReschedule();
  if(auto splash = getSplashScreenOverlay())
    splash->addMessage(string);
}

void SplashScreenUseCases::setSplashScreenMessage(const Glib::ustring &string)
{
  auto screensaver = m_settings.getSetting<ScreenSaverTimeoutSetting>();
  screensaver->endAndReschedule();
  if(auto splash = getSplashScreenOverlay())
    splash->setMessage(string);
}

void SplashScreenUseCases::finishSplashScreen()
{
  if(getSplashScreenOverlay())
  {
    auto &boled = m_hwui.getPanelUnit().getEditPanel().getBoled();
    boled.resetOverlay();
  }
}

SplashLayout *SplashScreenUseCases::getSplashScreenOverlay()
{
  if(!Application::get().isQuit())
  {
    if(auto overlayPtr = m_hwui.getPanelUnit().getEditPanel().getBoled().getOverlay())
    {
      return dynamic_cast<SplashLayout *>(overlayPtr.get());
    }
  }
  return nullptr;
}