#pragma once

#include "ParameterGroup.h"

class CombFilterGroup : public ParameterGroup
{
 public:
  CombFilterGroup(ParameterDualGroupSet *parent);
  virtual ~CombFilterGroup();

  void init();
};
