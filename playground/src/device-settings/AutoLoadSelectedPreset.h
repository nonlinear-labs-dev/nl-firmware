#pragma once

#include <nltools/enums/EnumTools.h>
#include <nltools/Types.h>
#include "EnumSetting.h"

ENUM(LoadPresetMode, uint8_t, PartSelect, PresetSelect, DirectLoad);

class AutoLoadSelectedPreset : public EnumSetting<LoadPresetMode>
{
 private:
  typedef EnumSetting<LoadPresetMode> super;

 public:
  explicit AutoLoadSelectedPreset(Settings& settings);

  bool set(tEnum m) override;

  AutoLoadSelectedPreset(const AutoLoadSelectedPreset& other) = delete;
  AutoLoadSelectedPreset& operator=(const AutoLoadSelectedPreset&) = delete;

  const std::vector<ustring>& enumToString() const override;
  const std::vector<ustring>& enumToDisplayString() const override;

  static void cycleForSoundType(SoundType type);
};
