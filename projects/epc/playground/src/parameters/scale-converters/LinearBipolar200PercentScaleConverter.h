#pragma once

#include "LinearScaleConverter.h"

class LinearBipolar200PercentScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar200PercentScaleConverter();
  ~LinearBipolar200PercentScaleConverter() override;

  [[nodiscard]] tControlPositionValue getCoarseDenominator(const QuantizedValue &v) const override;
  [[nodiscard]] tControlPositionValue getFineDenominator(const QuantizedValue &v) const override;
};

class LinearBipolar200PercentFineScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar200PercentFineScaleConverter();
  ~LinearBipolar200PercentFineScaleConverter() override;

  [[nodiscard]] tControlPositionValue getCoarseDenominator(const QuantizedValue &v) const override;
  [[nodiscard]] tControlPositionValue getFineDenominator(const QuantizedValue &v) const override;
};
