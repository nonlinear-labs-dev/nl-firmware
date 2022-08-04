#pragma once

#include "ScaleConverter.h"

class ParabolicGainDbScaleConverter : public ScaleConverter
{
 public:
  ParabolicGainDbScaleConverter();
  ~ParabolicGainDbScaleConverter() override;

  [[nodiscard]] bool isBiPolar() const override;

  [[nodiscard]] tControlPositionValue displayToControlPosition(const tDisplayValue &displayValue) const;
  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue &cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;
};
