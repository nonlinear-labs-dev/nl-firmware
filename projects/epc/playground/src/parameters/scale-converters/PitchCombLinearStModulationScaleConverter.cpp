#include "PitchCombLinearStModulationScaleConverter.h"
#include "dimension/PitchDimension.h"

PitchCombLinearStModulationScaleConverter::PitchCombLinearStModulationScaleConverter()
    : LinearScaleConverter(tDisplayRange(-120, 120), PitchDimension::get())
{
}

PitchCombLinearStModulationScaleConverter::~PitchCombLinearStModulationScaleConverter()
{
}
