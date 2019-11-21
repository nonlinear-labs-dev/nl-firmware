#pragma once
#include "ParameterGroup.h"

class GlobalParameterGroups : public ParameterGroup
{
 public:
  GlobalParameterGroups(ParameterDualGroupSet *parent);
  void init() override;
};
