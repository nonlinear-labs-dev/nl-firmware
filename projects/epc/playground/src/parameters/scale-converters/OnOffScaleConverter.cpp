#include "OnOffScaleConverter.h"

tDisplayValue OnOffScaleConverter::controlPositionToDisplay(const tControlPositionValue& cpValue) const
{
  return cpValue;
}

bool OnOffScaleConverter::isBiPolar() const
{
  return false;
}
