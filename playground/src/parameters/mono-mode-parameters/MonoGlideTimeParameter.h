#pragma once
#include "ModulateableMonoParameter.h"

class MonoGlideTimeParameter : public ModulateableMonoParameter
{
 public:
  MonoGlideTimeParameter(ParameterGroup* group, const ParameterId& id);

  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
};
