#pragma once
#include "ParameterGroup.h"

class GlobalParameterGroups : public ParameterGroup
{
 public:
  GlobalParameterGroups(ParameterDualGroupSet *parent);
  void init() override;

  static bool isSplitPoint(const Parameter* p);
  static bool isSplitPoint(const ParameterId& id);
};
