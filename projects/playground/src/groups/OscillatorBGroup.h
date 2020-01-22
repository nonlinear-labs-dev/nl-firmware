#pragma once

#include "ParameterGroup.h"

class OscillatorBGroup : public ParameterGroup
{
 public:
  OscillatorBGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~OscillatorBGroup();

  void init();
};
