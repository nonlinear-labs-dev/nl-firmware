#pragma once

#include "ParameterGroup.h"

class GapFilterGroup : public ParameterGroup
{
 public:
  GapFilterGroup(ParameterDualGroupSet *parent);
  virtual ~GapFilterGroup();

  void init();
};
