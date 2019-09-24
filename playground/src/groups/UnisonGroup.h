#pragma once

#include "ParameterGroup.h"

class UnisonGroup : public ParameterGroup
{
 public:
  UnisonGroup(ParameterDualGroupSet *parent);
  virtual ~UnisonGroup();

  void init();
};
