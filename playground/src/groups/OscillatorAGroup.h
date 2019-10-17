#pragma once

#include "ParameterGroup.h"

class OscillatorAGroup : public ParameterGroup
{
 public:
  OscillatorAGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~OscillatorAGroup();

  void init();
};
