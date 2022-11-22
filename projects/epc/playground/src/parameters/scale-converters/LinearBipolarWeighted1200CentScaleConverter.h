#pragma once

#include "LinearScaleConverter.h"

class LinearBipolarWeighted1200CentScaleConverter : public ScaleConverter
{
 public:
  LinearBipolarWeighted1200CentScaleConverter();
  ~LinearBipolarWeighted1200CentScaleConverter() override;
  tDisplayValue controlPositionToDisplay(const tControlPositionValue& cpValue) const override;
  Glib::ustring controlPositionToDisplayJS() const override;
  bool isBiPolar() const override;

 private:
  tDisplayRange m_displayRange;
};

