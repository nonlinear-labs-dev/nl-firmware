#include <Application.h>
#include <device-settings/AutoLoadSelectedPreset.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <presets/PresetManager.h>

AutoLoadSelectedPreset::AutoLoadSelectedPreset(Settings &settings)
    : super(settings, LoadPresetMode::PresetSelect)
{
}

const std::vector<ustring> &AutoLoadSelectedPreset::enumToString() const
{
  static auto strs = getAllStrings<LoadPresetMode>();
  static auto ret = std::vector<Glib::ustring>(strs.begin(), strs.end());
  return ret;
}

const std::vector<ustring> &AutoLoadSelectedPreset::enumToDisplayString() const
{
  static auto ret = std::vector<Glib::ustring>{ "Select Part", "Select Preset", "Direct Load" };
  return ret;
}

bool AutoLoadSelectedPreset::set(tEnum m)
{
  bool ret = super::set(m);

  if(m == LoadPresetMode::DirectLoad && !getSettings()->isLoading())
  {
    Application::get().getPresetManager()->doAutoLoadSelectedPreset();
  }

  return ret;
}

void AutoLoadSelectedPreset::cycleForSoundType(SoundType type)
{
  auto setting = Application::get().getSettings()->getSetting<AutoLoadSelectedPreset>();

  if(type == SoundType::Single)
  {
    switch(setting->get())
    {
      case LoadPresetMode::PresetSelect:
      case LoadPresetMode::PartSelect:
        setting->set(LoadPresetMode::DirectLoad);
        break;
      case LoadPresetMode::DirectLoad:
        setting->set(LoadPresetMode::PresetSelect);
        break;
    }
  }
  else
  {
    switch(setting->get())
    {
      case LoadPresetMode::PartSelect:
        setting->set(LoadPresetMode::PresetSelect);
        break;
      case LoadPresetMode::PresetSelect:
        setting->set(LoadPresetMode::DirectLoad);
        break;
      case LoadPresetMode::DirectLoad:
        setting->set(LoadPresetMode::PartSelect);
        break;
    }
  }
}
