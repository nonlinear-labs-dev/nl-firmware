#pragma once
#include "MonoParameter.h"

class MonoGlideTimeParameter : public MonoParameter
{
 public:
  MonoGlideTimeParameter(ParameterGroup* group, Parameter::ID id);

  ustring getLongName() const override;

  ustring getShortName() const override;
};