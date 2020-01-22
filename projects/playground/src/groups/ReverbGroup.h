#pragma once

#include "ParameterGroup.h"

class ReverbGroup : public ParameterGroup
{
 public:
  ReverbGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ReverbGroup();

  void init();
};
