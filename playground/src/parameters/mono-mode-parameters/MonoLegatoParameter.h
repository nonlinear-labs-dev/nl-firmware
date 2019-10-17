#pragma once

#include "MonoParameter.h"

class MonoLegatoParameter : public MonoParameter
{
 public:
  MonoLegatoParameter(ParameterGroup *group, uint16_t id);
  ustring getDisplayString() const override;

  ustring getLongName() const override;
  ustring getShortName() const override;
};