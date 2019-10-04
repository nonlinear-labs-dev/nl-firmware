#pragma once
#include <presets/PresetParameterGroup.h>

class SplitPresetParameterGroup : public PresetParameterGroup {
public:
  SplitPresetParameterGroup();
  explicit SplitPresetParameterGroup(const ::ParameterGroup &other);
  explicit SplitPresetParameterGroup(const PresetParameterGroup &other);
};
