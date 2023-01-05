#pragma once

#include "ParameterGroup.h"

class SVFilterGroup : public ParameterGroup
{
 public:
  SVFilterGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
