#pragma once

#include "ParameterGroup.h"

class CabinetGroup : public ParameterGroup
{
 public:
  CabinetGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~CabinetGroup();

  void init();
};
