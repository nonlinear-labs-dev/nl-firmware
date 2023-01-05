#pragma once

#include "ParameterGroup.h"

class FBMixerGroup : public ParameterGroup
{
 public:
  FBMixerGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
