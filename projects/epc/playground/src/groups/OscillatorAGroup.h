#pragma once

#include "ParameterGroup.h"

class OscillatorAGroup : public ParameterGroup
{
 public:
  OscillatorAGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
