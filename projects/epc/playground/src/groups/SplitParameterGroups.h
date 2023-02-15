#pragma once
#include "ParameterGroup.h"

class SplitParameterGroups : public ParameterGroup
{
 public:
  SplitParameterGroups(ParameterGroupSet* parent, VoiceGroup vg);

  static bool isSplitPoint(const Parameter* p);
  static bool isSplitPoint(const ParameterId& id);
};
