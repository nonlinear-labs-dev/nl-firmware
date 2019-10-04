#pragma once
#include "ParameterGroup.h"

class SplitSoundGroup : public ParameterGroup
{
 public:
  SplitSoundGroup(ParameterDualGroupSet *parent);
  void init() override;
};
