#pragma once

#include "ParameterGroup.h"

class FlangerGroup : public ParameterGroup
{
 public:
  FlangerGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~FlangerGroup();

  void init();
};
