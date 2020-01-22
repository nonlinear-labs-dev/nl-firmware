#pragma once

#include "ParameterGroup.h"

class FBMixerGroup : public ParameterGroup
{
 public:
  FBMixerGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~FBMixerGroup();

  void init();
};
