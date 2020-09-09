#pragma once

#include "ParameterGroup.h"

class ShaperAGroup : public ParameterGroup
{
 public:
  ShaperAGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ShaperAGroup();

  void init();
};
