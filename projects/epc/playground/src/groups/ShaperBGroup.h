#pragma once

#include "ParameterGroup.h"

class ShaperBGroup : public ParameterGroup
{
 public:
  ShaperBGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ShaperBGroup();

  void init();
};
