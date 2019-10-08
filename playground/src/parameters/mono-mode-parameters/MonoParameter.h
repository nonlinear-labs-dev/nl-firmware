#pragma once
#include <parameters/Parameter.h>

class MonoParameter : public Parameter
{
 public:
  MonoParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling, tControlPositionValue def,
                tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator);
};
