#pragma once

#include "ParameterGroup.h"

class ReverbGroup : public ParameterGroup
{
 public:
  ReverbGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
};
