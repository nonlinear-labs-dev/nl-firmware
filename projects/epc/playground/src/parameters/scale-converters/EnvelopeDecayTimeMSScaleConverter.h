#pragma once

#include "ScaleConverter.h"

class EnvelopeDecayTimeMSScaleConverter : public ScaleConverter
{
  typedef ScaleConverter super;

 public:
  EnvelopeDecayTimeMSScaleConverter();
  ~EnvelopeDecayTimeMSScaleConverter() override;

  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue &cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;
  [[nodiscard]] bool isBiPolar() const override;

 private:
  tDisplayRange m_displayRange;
};
