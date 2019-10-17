#pragma once
#include "MonoParameter.h"

class MonoPriorityParameter : public MonoParameter {
public:
  MonoPriorityParameter(ParameterGroup* parent, Parameter::ID id);
  ustring getDisplayString() const override;

  ustring getLongName() const override;

  ustring getShortName() const override;
};