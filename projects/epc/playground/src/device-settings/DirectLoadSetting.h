#pragma once
#include <presets/PresetPartSelection.h>
#include "BooleanSetting.h"

class DirectLoadSetting : public BooleanSetting
{
 public:
  explicit DirectLoadSetting(Settings& settings);

  DirectLoadSetting(const DirectLoadSetting& other) = delete;
  DirectLoadSetting& operator=(const DirectLoadSetting&) = delete;

 private:
  using BooleanSetting::set;
  using BooleanSetting::toggle;

  friend class DirectLoadUseCases;
};
