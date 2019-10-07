#pragma once
#include <presets/PresetParameterGroup.h>
class EditBuffer;

class SplitPresetParameterGroup : public PresetParameterGroup {
public:
  SplitPresetParameterGroup();
  explicit SplitPresetParameterGroup(const SplitPresetParameterGroup* p);
  explicit SplitPresetParameterGroup(const EditBuffer& eb);
};
