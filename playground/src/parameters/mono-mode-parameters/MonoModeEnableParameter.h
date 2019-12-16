#pragma once
#include "UnmodulateableMonoParameter.h"

class MonoModeEnableParameter : public UnmodulateableMonoParameter
{
 public:
  MonoModeEnableParameter(ParameterGroup* group, const ParameterId& id);

  Glib::ustring getDisplayString() const override;
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
};
