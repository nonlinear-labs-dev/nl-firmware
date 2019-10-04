#pragma once
#include "Parameter.h"

class SplitPointParameter : public Parameter {
public:
  SplitPointParameter(ParameterGroup *group, uint16_t id, tControlPositionValue def);
  std::string getDisplayValue(VoiceGroup vg) const;
};
