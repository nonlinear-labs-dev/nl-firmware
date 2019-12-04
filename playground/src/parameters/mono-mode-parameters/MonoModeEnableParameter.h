#pragma once
#include "MonoParameter.h"

class MonoModeEnableParameter : public MonoParameter
{
 public:
  MonoModeEnableParameter(ParameterGroup *group, ParameterId id);
  Glib::ustring getDisplayString() const override;

  Glib::ustring getLongName() const override;

  Glib::ustring getShortName() const override;
};
