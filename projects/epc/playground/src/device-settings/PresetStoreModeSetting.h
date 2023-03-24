#pragma once

#include "EnumSetting.h"
#include "nltools/Types.h"

class Settings;
class Parameter;

class PresetStoreModeSetting : public EnumSetting<PresetStoreModeSettings>
{
 private:
  typedef EnumSetting<PresetStoreModeSettings> super;

 public:
  explicit PresetStoreModeSetting(UpdateDocumentContributor &settings);
  ~PresetStoreModeSetting() override;

  PresetStoreModeSetting(const PresetStoreModeSetting &other) = delete;
  PresetStoreModeSetting &operator=(const PresetStoreModeSetting &) = delete;

 private:
  const std::vector<Glib::ustring> &enumToString() const override;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;
};
