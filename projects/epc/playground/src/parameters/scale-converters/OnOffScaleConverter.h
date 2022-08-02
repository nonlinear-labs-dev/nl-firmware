#pragma once
#include "dimension/OnOffDimension.h"
#include "LinearCountScaleConverter.h"

class OnOffScaleConverter : public LinearCountScaleConverter<2, OnOffDimension>
{
 public:
  [[nodiscard]] tDisplayValue controlPositionToDisplay(const tControlPositionValue& cpValue) const override;
  [[nodiscard]] bool isBiPolar() const override;
};
