#pragma once

#include "ParameterGroup.h"

class GapFilterGroup : public ParameterGroup
{
 public:
  GapFilterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~GapFilterGroup();

  void init();
};
