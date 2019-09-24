#pragma once

#include "ParameterGroup.h"

class FBMixerGroup : public ParameterGroup
{
 public:
  FBMixerGroup(ParameterDualGroupSet *parent);
  virtual ~FBMixerGroup();

  void init();
};
