#pragma once

#include "ParameterGroup.h"

class UnisonGroup : public ParameterGroup
{
 public:
  UnisonGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~UnisonGroup();

  void init();
};
