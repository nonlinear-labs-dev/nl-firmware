#pragma once

#include "ParameterGroup.h"

class OscillatorBGroup : public ParameterGroup
{
 public:
  OscillatorBGroup(ParameterDualGroupSet *parent);
  virtual ~OscillatorBGroup();

  void init();
};
