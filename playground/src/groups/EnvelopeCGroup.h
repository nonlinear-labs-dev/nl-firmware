#pragma once

#include "ParameterGroup.h"

class EnvelopeCGroup : public ParameterGroup
{
 public:
  EnvelopeCGroup(ParameterDualGroupSet *parent);
  virtual ~EnvelopeCGroup();

  void init();
};
