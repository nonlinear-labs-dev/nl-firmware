#pragma once
#include "ParameterGroup.h"

class GlobalParameterGroups : public ParameterGroup
{
 public:
  GlobalParameterGroups(ParameterGroupSet *parent);
  void init() override;

  static bool isSplitPoint(const Parameter* p);
  static bool isSplitPoint(const ParameterId& id);
};
