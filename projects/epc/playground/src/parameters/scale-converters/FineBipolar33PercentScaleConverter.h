#pragma once

#include "LinearScaleConverter.h"

class FineBipolar33PercentScaleConverter : public LinearScaleConverter
{
 public:
  FineBipolar33PercentScaleConverter();
  ~FineBipolar33PercentScaleConverter() override;
};
