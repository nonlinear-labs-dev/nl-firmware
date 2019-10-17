#pragma once

#include "ParameterGroup.h"

class EnvelopeBGroup : public ParameterGroup
{
 public:
  EnvelopeBGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~EnvelopeBGroup();

  void init();
};
