#pragma once

#include "ParameterGroup.h"

class MonoGroup : public ParameterGroup
{
public:
  MonoGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  void init();
};