#pragma once

#include "ParameterGroup.h"

class CombFilterGroup : public ParameterGroup
{
 public:
  CombFilterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~CombFilterGroup();

  void init();
};
