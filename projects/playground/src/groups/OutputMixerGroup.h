#pragma once

#include "ParameterGroup.h"

class OutputMixerGroup : public ParameterGroup
{
 public:
  OutputMixerGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~OutputMixerGroup();

  void init();
};
