#pragma once
#include <parameters/scale-converters/LinearScaleConverter.h>

class FineBipolar200StScaleConverter : public LinearScaleConverter
{
 public:
  FineBipolar200StScaleConverter();
  ~FineBipolar200StScaleConverter() noexcept override;
};
