#pragma once

#include "ParameterGroup.h"

class OutputMixerGroup : public ParameterGroup
{
 public:
  OutputMixerGroup(ParameterDualGroupSet *parent);
  virtual ~OutputMixerGroup();

  void init();
};
