#pragma once

#include "ParameterGroup.h"

class OscillatorAGroup : public ParameterGroup
{
 public:
  OscillatorAGroup(ParameterDualGroupSet *parent);
  virtual ~OscillatorAGroup();

  void init();
};
