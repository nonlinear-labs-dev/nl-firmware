#pragma once

#include "ParameterGroup.h"

class SVFilterGroup : public ParameterGroup
{
 public:
  SVFilterGroup(ParameterDualGroupSet *parent);
  virtual ~SVFilterGroup();

  void init();
};
