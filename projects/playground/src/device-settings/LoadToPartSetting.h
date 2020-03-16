#pragma once
#include "BooleanSetting.h"

class LoadToPartSetting : public BooleanSetting
{
 public:
  explicit LoadToPartSetting(Settings& settings);

  LoadToPartSetting(const LoadToPartSetting& other) = delete;
  LoadToPartSetting& operator=(const LoadToPartSetting&) = delete;
};