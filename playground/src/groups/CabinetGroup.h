#pragma once

#include "ParameterGroup.h"

class CabinetGroup : public ParameterGroup
{
 public:
  CabinetGroup(ParameterDualGroupSet *parent);
  virtual ~CabinetGroup();

  void init();
};
