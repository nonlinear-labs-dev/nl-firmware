#pragma once

#include <nltools/enums/EnumTools.h>
#include <nltools/Types.h>
#include "EnumSetting.h"

ENUM(LoadMode, uint8_t, LoadToPart, Select, DirectLoad);

class LoadModeSetting : public EnumSetting<LoadMode>
{
 private:
  typedef EnumSetting<LoadMode> super;

 public:
  explicit LoadModeSetting(Settings& settings);

  bool set(tEnum m) override;

  LoadModeSetting(const LoadModeSetting& other) = delete;
  LoadModeSetting& operator=(const LoadModeSetting&) = delete;

  const std::vector<ustring>& enumToString() const override;
  const std::vector<ustring>& enumToDisplayString() const override;

  void cycleForSoundType(SoundType type);
};
