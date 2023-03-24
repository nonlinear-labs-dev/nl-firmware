#pragma once

#include <parameters/Parameter.h>

class UnmodulateableMonoParameter : public Parameter
{
 public:
  UnmodulateableMonoParameter(ParameterGroup* group, const ParameterId& id);
  Layout* createLayout(FocusAndMode focusAndMode) const override;
};
