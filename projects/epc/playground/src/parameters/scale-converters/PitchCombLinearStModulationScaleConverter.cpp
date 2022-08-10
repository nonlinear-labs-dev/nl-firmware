#include "PitchCombLinearStModulationScaleConverter.h"
#include "dimension/PitchDimension.h"

PitchCombLinearStModulationScaleConverter::PitchCombLinearStModulationScaleConverter()
    : LinearScaleConverter(tDisplayRange(-120, 120), PitchDimension::get())
{
}

PitchCombLinearStModulationScaleConverter::~PitchCombLinearStModulationScaleConverter()
{
}

tControlPositionValue PitchCombLinearStModulationScaleConverter::getCoarseDenominator(const QuantizedValue &v) const
{
  return 120;
}

tControlPositionValue PitchCombLinearStModulationScaleConverter::getFineDenominator(const QuantizedValue &v) const
{
  return 6000;
}
