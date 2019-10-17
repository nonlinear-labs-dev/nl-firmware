#pragma once
#include "MonoParameter.h"

class MonoModeEnableParameter : public MonoParameter
{
 public:
  MonoModeEnableParameter(ParameterGroup *group, uint16_t id);
  ustring getDisplayString() const override;

  ustring getLongName() const override;

  ustring getShortName() const override;
};
