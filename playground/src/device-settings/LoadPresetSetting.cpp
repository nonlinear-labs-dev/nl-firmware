#include <Application.h>
#include <device-settings/LoadPresetSetting.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <presets/PresetManager.h>

LoadPresetSetting::LoadPresetSetting(Settings &settings)
    : super(settings, LoadMode::Select)
{
}

const std::vector<ustring> &LoadPresetSetting::enumToString() const
{
  static auto strs = getAllStrings<LoadMode>();
  static auto ret = std::vector<Glib::ustring>(strs.begin(), strs.end());
  return ret;
}

const std::vector<ustring> &LoadPresetSetting::enumToDisplayString() const
{
  static auto ret = std::vector<Glib::ustring>{ "Select Part", "Select Preset", "Direct Load" };
  return ret;
}

bool LoadPresetSetting::set(tEnum m)
{
  bool ret = super::set(m);

  if(m == LoadMode::DirectLoad && !getSettings()->isLoading())
  {
    Application::get().getPresetManager()->doAutoLoadSelectedPreset();
  }

  return ret;
}

void LoadPresetSetting::cycleForSoundType(SoundType type)
{
  auto setting = Application::get().getSettings()->getSetting<LoadPresetSetting>();

  if(type == SoundType::Single)
  {
    switch(setting->get())
    {
      case LoadMode::Select:
      case LoadMode::LoadToPart:
        setting->set(LoadMode::DirectLoad);
        break;
      case LoadMode::DirectLoad:
        setting->set(LoadMode::Select);
        break;
    }
  }
  else
  {
    switch(setting->get())
    {
      case LoadMode::LoadToPart:
        setting->set(LoadMode::Select);
        break;
      case LoadMode::Select:
        setting->set(LoadMode::DirectLoad);
        break;
      case LoadMode::DirectLoad:
        setting->set(LoadMode::LoadToPart);
        break;
    }
  }
}
