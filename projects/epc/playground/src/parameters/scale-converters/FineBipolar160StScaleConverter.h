#pragma once
#include <parameters/scale-converters/LinearScaleConverter.h>

class FineBipolar160StScaleConverter : public LinearScaleConverter
{
 public:
  FineBipolar160StScaleConverter();
  ~FineBipolar160StScaleConverter() noexcept override;
};
