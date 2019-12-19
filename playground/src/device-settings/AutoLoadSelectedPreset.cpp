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
  auto strs = getAllStrings<LoadPresetMode>();
  static auto ret = std::vector<Glib::ustring>(strs.begin(), strs.end());
  return ret;
}

const std::vector<ustring> &AutoLoadSelectedPreset::enumToDisplayString() const
{
  auto strs = getAllStrings<LoadPresetMode>();
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
