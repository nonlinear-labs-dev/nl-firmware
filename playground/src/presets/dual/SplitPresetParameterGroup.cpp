
#include "SplitPresetParameterGroup.h"

SplitPresetParameterGroup::SplitPresetParameterGroup() : PresetParameterGroup() {
  m_parameters[18700] = std::make_unique<PresetParameter>(18700);
}

SplitPresetParameterGroup::SplitPresetParameterGroup(const ::ParameterGroup &other) : PresetParameterGroup(other) {
}

SplitPresetParameterGroup::SplitPresetParameterGroup(const PresetParameterGroup &other) : PresetParameterGroup(other) {

}
