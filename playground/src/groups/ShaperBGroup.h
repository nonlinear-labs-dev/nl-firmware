#pragma once

#include "ParameterGroup.h"

class ShaperBGroup : public ParameterGroup
{
 public:
  ShaperBGroup(ParameterDualGroupSet *parent);
  virtual ~ShaperBGroup();

  void init();
};
