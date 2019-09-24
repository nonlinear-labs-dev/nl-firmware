#pragma once

#include "ParameterGroup.h"

class EnvelopeAGroup : public ParameterGroup
{
 public:
  EnvelopeAGroup(ParameterDualGroupSet *parent);
  virtual ~EnvelopeAGroup();

  void init();
};
