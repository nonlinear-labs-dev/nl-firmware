#pragma once
#include <parameters/Parameter.h>

class MonoParameter : public Parameter
{
 public:
  MonoParameter(ParameterGroup *group, ParameterId id, const ScaleConverter *scaling, tControlPositionValue def,
                tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator);

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
};
