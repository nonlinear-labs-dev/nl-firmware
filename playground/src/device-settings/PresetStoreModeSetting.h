#pragma once

#include "EnumSetting.h"

class Settings;
class Parameter;

enum class PresetStoreModeSettings
{
  PRESET_STORE_MODE_APPEND,
  PRESET_STORE_MODE_OVERWRITE,
  PRESET_STORE_MODE_INSERT
};

class PresetStoreModeSetting : public EnumSetting<PresetStoreModeSettings>
{
 private:
  typedef EnumSetting<PresetStoreModeSettings> super;

 public:
  PresetStoreModeSetting(Settings &settings);
  virtual ~PresetStoreModeSetting();

 private:
  PresetStoreModeSetting(const PresetStoreModeSetting &other);
  PresetStoreModeSetting &operator=(const PresetStoreModeSetting &);

  const std::vector<Glib::ustring> &enumToString() const override;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;
};
