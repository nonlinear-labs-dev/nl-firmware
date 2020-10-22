#include <Application.h>

#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>

#include "ScreenSaverTimeoutSetting.h"

ScreenSaverTimeoutSetting::ScreenSaverTimeoutSetting(UpdateDocumentContributor& parent)
    : Setting(parent)
{
}

void ScreenSaverTimeoutSetting::load(const Glib::ustring& text, Initiator initiator)
{
  m_timeout = std::chrono::minutes(std::stoi(text));
}

Glib::ustring ScreenSaverTimeoutSetting::save() const
{
  return std::to_string(m_timeout.count());
}

bool ScreenSaverTimeoutSetting::persistent() const
{
  return true;
}

Glib::ustring ScreenSaverTimeoutSetting::getDisplayString() const
{
  if(m_timeout.count() == 0)
    return "Disabled";
  if(m_timeout.count() == 1)
    return "1 Minute";
  if(m_timeout.count() == 60)
    return "1 Hour";
  if(m_timeout.count() > 60)
    return std::to_string(m_timeout.count() / 60) + " Hours";

  return std::to_string(m_timeout.count()) + " Minutes";
}

sigc::connection ScreenSaverTimeoutSetting::onScreenSaverStateChanged(sigc::slot<void, bool> s)
{
  return m_screenSaverSignal.connect(s);
}

void ScreenSaverTimeoutSetting::init()
{
  if(m_timeout.count() > 0)
  {
    m_expiration = std::make_unique<Expiration>([&] { m_screenSaverSignal.send(true); }, m_timeout);
  }

  //TODO subscribe to real events...

  Application::get().getHWUI()->onHWUIChanged([&]() {
    if(m_expiration)
    {
      m_expiration->refresh(m_timeout);
    }
  });
}

void ScreenSaverTimeoutSetting::sendState(bool state)
{
  m_screenSaverSignal.send(state);
}

void ScreenSaverTimeoutSetting::incDec(int inc)
{

  //  auto currentIndex = std::find(s_logTimeOuts.begin(), s_logTimeOuts.end(), m_timeout.count());
  //  if(currentIndex != s_logTimeOuts.end())
  //  {
  //    currentIndex = std::max(0, std::min<int>(currentIndex + inc, s_logTimeOuts.size() - 1));
  //  }

  auto n = m_timeout.count() + inc;
  n = std::min<int>(std::max<int>(n, 0), 30);
  m_timeout = std::chrono::minutes(n);

  if(n == 0)
  {
    m_expiration.reset(nullptr);
    m_screenSaverSignal.send(false);
  }
  else
  {
    if(m_expiration)
    {
      m_expiration->refresh(m_timeout);
    }
    else
    {
      m_expiration = std::make_unique<Expiration>([&] { m_screenSaverSignal.send(true); }, m_timeout);
    }
  }

  notify();
}
