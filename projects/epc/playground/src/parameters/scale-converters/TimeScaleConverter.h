#pragma once

#include "ScaleConverter.h"

class TimeScaleConverter : public ScaleConverter
{
  typedef ScaleConverter super;

 public:
  TimeScaleConverter(const tDisplayRange& displayRange);
  ~TimeScaleConverter() override;

  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue& cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;
  [[nodiscard]] bool isBiPolar() const override;

  [[nodiscard]] size_t hash() const override;

 private:
  tDisplayRange m_displayRange;
};
