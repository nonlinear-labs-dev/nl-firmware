#include <Application.h>
#include <device-settings/LoadModeSetting.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <presets/PresetManager.h>
#include <libundo/undo/StringTools.h>

LoadModeSetting::LoadModeSetting(Settings &settings)
    : super(settings, LoadMode::Select)
    , m_singleSoundOrder { LoadMode::Select, LoadMode::DirectLoad }
    , m_dualSoundOrder { LoadMode::LoadToPart, LoadMode::Select, LoadMode::DirectLoad }
{
}

const std::vector<Glib::ustring> &LoadModeSetting::enumToString() const
{
  static auto strs = getAllStrings<LoadMode>();
  static auto ret = std::vector<Glib::ustring>(strs.begin(), strs.end());
  return ret;
}

const std::vector<Glib::ustring> &LoadModeSetting::enumToDisplayString() const
{
  static auto ret = std::vector<Glib::ustring> { "Select Part", "Select only", "Direct Load" };
  return ret;
}

void LoadModeSetting::cycleForSoundType(SoundType type)
{
  if(type == SoundType::Single)
    updateLoadMode(get(), 1, m_singleSoundOrder);
  else
    updateLoadMode(get(), 1, m_dualSoundOrder);
}

void LoadModeSetting::antiCycleForSoundType(SoundType type)
{
  if(type == SoundType::Single)
    updateLoadMode(get(), -1, m_singleSoundOrder);
  else
    updateLoadMode(get(), -1, m_dualSoundOrder);
}

template <typename T> void LoadModeSetting::updateLoadMode(LoadMode current, int dir, const T &loadModeOrder)
{
  auto currentIt = std::find(loadModeOrder.begin(), loadModeOrder.end(), current);

  if(currentIt == loadModeOrder.end())
    currentIt = loadModeOrder.begin();

  auto idx = std::distance(loadModeOrder.begin(), currentIt);
  auto targetIdx = idx + dir;

  if(size_t(targetIdx) == loadModeOrder.size())
    targetIdx = 0;
  else if(targetIdx < 0)
    targetIdx = ssize_t(loadModeOrder.size() - 1);

  set(loadModeOrder[size_t(targetIdx)]);
}

Glib::ustring LoadModeSetting::getDisplayStringForVoiceGroup(VoiceGroup vg) const
{
  return getDisplayStringForVoiceGroup(vg, get());
}

Glib::ustring LoadModeSetting::getDisplayStringForVoiceGroup(VoiceGroup vg, LoadMode setting) const
{
  switch(setting)
  {
    case LoadMode::Select:
    case LoadMode::DirectLoad:
      return enumToDisplayString()[static_cast<int>(setting)];
    case LoadMode::LoadToPart:
      return UNDO::StringTools::buildString("Load to ", toString(vg));
  }

  return "";
}
