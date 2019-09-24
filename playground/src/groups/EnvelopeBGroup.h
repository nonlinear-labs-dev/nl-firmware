#pragma once

#include "ParameterGroup.h"

class EnvelopeBGroup : public ParameterGroup
{
 public:
  EnvelopeBGroup(ParameterDualGroupSet *parent);
  virtual ~EnvelopeBGroup();

  void init();
};
