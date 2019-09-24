#pragma once

#include "ParameterGroup.h"

class EchoGroup : public ParameterGroup
{
 public:
  EchoGroup(ParameterDualGroupSet *parent);
  virtual ~EchoGroup();

  void init();
};
