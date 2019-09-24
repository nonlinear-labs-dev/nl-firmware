#pragma once

#include "ParameterGroup.h"

class ReverbGroup : public ParameterGroup
{
 public:
  ReverbGroup(ParameterDualGroupSet *parent);
  virtual ~ReverbGroup();

  void init();
};
