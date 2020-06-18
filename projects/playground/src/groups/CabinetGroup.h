#pragma once

#include "ParameterGroup.h"

class CabinetGroup : public ParameterGroup
{
 public:
  CabinetGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  virtual ~CabinetGroup();

  void init();
};
