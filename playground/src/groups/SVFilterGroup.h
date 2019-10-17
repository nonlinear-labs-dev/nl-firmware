#pragma once

#include "ParameterGroup.h"

class SVFilterGroup : public ParameterGroup
{
 public:
  SVFilterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~SVFilterGroup();

  void init();
};
