#pragma once
#include <parameters/ModulateableParameter.h>

class ModulateableMonoParameter : public ModulateableParameter
{
 public:
  ModulateableMonoParameter(ParameterGroup* group, const ParameterId& id, const ScaleConverter* scaling);

  Layout *createLayout(FocusAndMode focusAndMode) const override;
};
