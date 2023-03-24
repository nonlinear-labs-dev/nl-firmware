#include "SplitPointSyncParameters.h"
#include <device-settings/Settings.h>
#include <Application.h>
#include <parameters/Parameter.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

SplitPointSyncParameters::SplitPointSyncParameters(Settings& s)
    : Setting(s)
{
}

void SplitPointSyncParameters::init()
{
  if(Application::exists())
  {
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    eb->onEditBufferConverted(sigc::mem_fun(this, &SplitPointSyncParameters::onSoundConverted));
    Setting::init();
  }
}

void SplitPointSyncParameters::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  auto c_str = std::get<const char* const>(val);
  setState(strcmp(c_str, "on") == 0);
}

void SplitPointSyncParameters::undoableSet(UNDO::Transaction* transaction, bool newState)
{
  auto swap = UNDO::createSwapData(newState);
  transaction->addSimpleCommand(
      [this, swap](auto)
      {
        swap->swapWith(m_state);
        notify();
      });
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
