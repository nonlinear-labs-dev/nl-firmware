#pragma once

#include "ParameterGroup.h"

class MonoGroup : public ParameterGroup
{
 public:
  MonoGroup(ParameterGroupSet* parent, VoiceGroup voicegroup);
  static bool isMonoParameter(const ParameterId& id);
  static bool isMonoParameter(const Parameter* param);
};