#pragma once

#include "ScaleConverter.h"

class EnvelopeAttackDecayTimeMSScaleConverter : public ScaleConverter
{
  typedef ScaleConverter super;

 public:
  EnvelopeAttackDecayTimeMSScaleConverter();
  ~EnvelopeAttackDecayTimeMSScaleConverter() override;

  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue &cpValue) const override;
  [[nodiscard]] Glib::ustring controlPositionToDisplayJS() const override;
  [[nodiscard]] bool isBiPolar() const override;
};
