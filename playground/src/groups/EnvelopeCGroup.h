#pragma once

#include "ParameterGroup.h"

class EnvelopeCGroup : public ParameterGroup
{
 public:
  EnvelopeCGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~EnvelopeCGroup();

  void init();
};
