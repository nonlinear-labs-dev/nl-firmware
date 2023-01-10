#pragma once

#include "ParameterGroup.h"

class CombFilterGroup : public ParameterGroup
{
 public:
  CombFilterGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
