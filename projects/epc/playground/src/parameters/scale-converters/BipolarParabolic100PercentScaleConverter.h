#pragma once

#include "ParabolicScaleConverter.h"

class BipolarParabolic100PercentScaleConverter : public ScaleConverter
{
 public:
  BipolarParabolic100PercentScaleConverter();
  ~BipolarParabolic100PercentScaleConverter() override;

  [[nodiscard]] bool isBiPolar() const override;
  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue &cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;

 private:
  tDisplayRange m_displayRange;
};
