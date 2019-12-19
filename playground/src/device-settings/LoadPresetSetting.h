#pragma once

#include <nltools/enums/EnumTools.h>
#include <nltools/Types.h>
#include "EnumSetting.h"

ENUM(LoadMode, uint8_t, LoadToPart, Select, DirectLoad);

class LoadPresetSetting : public EnumSetting<LoadMode>
{
 private:
  typedef EnumSetting<LoadMode> super;

 public:
  explicit LoadPresetSetting(Settings& settings);

  bool set(tEnum m) override;

  LoadPresetSetting(const LoadPresetSetting& other) = delete;
  LoadPresetSetting& operator=(const LoadPresetSetting&) = delete;

  const std::vector<ustring>& enumToString() const override;
  const std::vector<ustring>& enumToDisplayString() const override;

  static void cycleForSoundType(SoundType type);
};
