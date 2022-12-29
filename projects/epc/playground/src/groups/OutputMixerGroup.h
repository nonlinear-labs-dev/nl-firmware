#pragma once

#include "ParameterGroup.h"

class OutputMixerGroup : public ParameterGroup
{
 public:
  OutputMixerGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
