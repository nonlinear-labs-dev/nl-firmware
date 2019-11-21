#pragma once
#include "MonoParameter.h"

class MonoGlideTimeParameter : public MonoParameter
{
 public:
  MonoGlideTimeParameter(ParameterGroup* group, ParameterId id);

  ustring getLongName() const override;

  ustring getShortName() const override;
};
