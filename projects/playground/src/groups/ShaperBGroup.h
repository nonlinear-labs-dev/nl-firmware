#pragma once

#include "ParameterGroup.h"

class ShaperBGroup : public ParameterGroup
{
 public:
  ShaperBGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ShaperBGroup();

  void init();
};
