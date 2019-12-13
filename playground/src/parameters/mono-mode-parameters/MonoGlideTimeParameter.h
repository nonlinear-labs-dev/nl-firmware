#pragma once
#include "MonoParameter.h"
#include "parameters/ModulateableParameter.h"

class MonoGlideTimeParameter : public ModulateableParameter
{
 public:
  MonoGlideTimeParameter(ParameterGroup* group, ParameterId id);

  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
  DFBLayout* createLayout(FocusAndMode focusAndMode) const override;
};
