#pragma once
#include "MonoParameter.h"

class MonoModeEnableParameter : public MonoParameter
{
 public:
  MonoModeEnableParameter(ParameterGroup *group, ParameterId id);
  ustring getDisplayString() const override;

  ustring getLongName() const override;

  ustring getShortName() const override;
};
