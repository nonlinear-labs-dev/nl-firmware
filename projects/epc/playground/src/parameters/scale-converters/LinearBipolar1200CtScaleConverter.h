#pragma once

#include "LinearScaleConverter.h"

class LinearBipolar1200CtScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar1200CtScaleConverter();
  ~LinearBipolar1200CtScaleConverter() override;
};

class LinearBipolar2Sections1200CentScaleConverter : public ScaleConverter
{
 public:
  LinearBipolar2Sections1200CentScaleConverter();
  ~LinearBipolar2Sections1200CentScaleConverter() override;
  tDisplayValue controlPositionToDisplay(const tControlPositionValue& cpValue) const override;
  Glib::ustring controlPositionToDisplayJS() const override;
  bool isBiPolar() const override;

 private:
  tDisplayRange m_displayRange;
};