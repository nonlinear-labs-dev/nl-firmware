#pragma once

#include "ParameterGroup.h"

class EchoGroup : public ParameterGroup
{
 public:
  EchoGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~EchoGroup();

  void init();
};
