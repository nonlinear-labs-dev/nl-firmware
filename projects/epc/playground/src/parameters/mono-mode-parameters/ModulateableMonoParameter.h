#pragma once
#include <parameters/ModulateableParameter.h>

class ModulateableMonoParameter : public ModulateableParameter
{
 public:
  ModulateableMonoParameter(ParameterGroup* group, const ParameterId& id);
  Layout* createLayout(FocusAndMode focusAndMode) const override;
};
