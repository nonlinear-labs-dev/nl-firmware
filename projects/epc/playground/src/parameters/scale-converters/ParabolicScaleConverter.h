#pragma once

#include "ScaleConverter.h"

class ParabolicScaleConverter : public ScaleConverter
{
 public:
  ParabolicScaleConverter(tDisplayRange displayRange, const Dimension& dimension);
  ~ParabolicScaleConverter() override;

  [[nodiscard]] bool isBiPolar() const override;
  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue& cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;

 private:
  tDisplayRange m_displayRange;
};
