#pragma once

#include "ParameterGroup.h"

class ShaperAGroup : public ParameterGroup
{
 public:
  ShaperAGroup(ParameterDualGroupSet *parent);
  virtual ~ShaperAGroup();

  void init();
};
