#include <Application.h>
#include <device-settings/LoadModeSetting.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <presets/PresetManager.h>

LoadModeSetting::LoadModeSetting(Settings &settings)
    : super(settings, LoadMode::Select)
{
}

const std::vector<ustring> &LoadModeSetting::enumToString() const
{
  static auto strs = getAllStrings<LoadMode>();
  static auto ret = std::vector<Glib::ustring>(strs.begin(), strs.end());
  return ret;
}

const std::vector<ustring> &LoadModeSetting::enumToDisplayString() const
{
  static auto ret = std::vector<Glib::ustring>{ "Select Part", "Select Preset", "Direct Load" };
  return ret;
}

bool LoadModeSetting::set(tEnum m)
{
  bool ret = super::set(m);

  if(m == LoadMode::DirectLoad && !getSettings()->isLoading())
  {
    Application::get().getPresetManager()->doAutoLoadSelectedPreset();
  }

  return ret;
}

void LoadModeSetting::cycleForSoundType(SoundType type)
{
  if(type == SoundType::Single)
  {
    switch(get())
    {
      case LoadMode::Select:
      case LoadMode::LoadToPart:
        set(LoadMode::DirectLoad);
        break;
      case LoadMode::DirectLoad:
        set(LoadMode::Select);
        break;
    }
  }
  else
  {
    switch(get())
    {
      case LoadMode::LoadToPart:
        set(LoadMode::Select);
        break;
      case LoadMode::Select:
        set(LoadMode::DirectLoad);
        break;
      case LoadMode::DirectLoad:
        set(LoadMode::LoadToPart);
        break;
    }
  }
}
