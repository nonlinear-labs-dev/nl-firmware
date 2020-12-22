#include "SplitPointSyncParameters.h"
#include <device-settings/Settings.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

SplitPointSyncParameters::SplitPointSyncParameters(Settings& s)
    : Setting(s)
{
}

void SplitPointSyncParameters::init()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onEditBufferConverted(sigc::mem_fun(this, &SplitPointSyncParameters::onSoundConverted));
  Setting::init();
}

void SplitPointSyncParameters::load(const Glib::ustring& text, Initiator initiator)
{
  m_state = text == "on";
  notify();
}

Glib::ustring SplitPointSyncParameters::save() const
{
  return m_state ? "on" : "off";
}

Glib::ustring SplitPointSyncParameters::getDisplayString() const
{
  return m_state ? "On" : "Off";
}

bool SplitPointSyncParameters::get() const
{
  return m_state;
}

void SplitPointSyncParameters::setState(bool s)
{
  m_state = s;
  notify();
}

void SplitPointSyncParameters::onSoundConverted(SoundType type)
{
  if(type == SoundType::Split)
  {
    setState(true);
  }
}
