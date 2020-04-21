#include "DirectLoadSetting.h"
#include <device-settings/Settings.h>
#include <Application.h>
#include <presets/PresetManager.h>

DirectLoadSetting::DirectLoadSetting(Settings &settings)
    : BooleanSetting(settings, false)
{
}

bool DirectLoadSetting::set(BooleanSettings m)
{
  auto ret = EnumSetting::set(m);
  if(auto pm = Application::get().getPresetManager())
  {
    if(m == BooleanSettings::BOOLEAN_SETTING_TRUE)
      pm->doAutoLoadSelectedPreset();
  }
}
