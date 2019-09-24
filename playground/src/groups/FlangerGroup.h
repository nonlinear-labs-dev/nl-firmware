#pragma once

#include "ParameterGroup.h"

class FlangerGroup : public ParameterGroup
{
 public:
  FlangerGroup(ParameterDualGroupSet *parent);
  virtual ~FlangerGroup();

  void init();
};
