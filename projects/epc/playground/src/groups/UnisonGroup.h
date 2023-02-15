#pragma once

#include "ParameterGroup.h"

class UnisonGroup : public ParameterGroup
{
 public:
  UnisonGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);

  static bool isUnisonParameter(const Parameter *parameter);
  static bool isUnisonParameter(const ParameterId &id);
};
