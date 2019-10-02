#pragma once

#include "ParameterGroup.h"

class ShaperAGroup : public ParameterGroup
{
 public:
  ShaperAGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ShaperAGroup();

  void init();
};
