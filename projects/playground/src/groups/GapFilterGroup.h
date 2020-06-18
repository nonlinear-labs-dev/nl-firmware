#pragma once

#include "ParameterGroup.h"

class GapFilterGroup : public ParameterGroup
{
 public:
  GapFilterGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  virtual ~GapFilterGroup();

  void init();
};
