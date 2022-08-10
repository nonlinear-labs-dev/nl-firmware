#include "PitchCombLinearStScaleConverter.h"
#include "dimension/PitchDimension.h"

PitchCombLinearStScaleConverter::PitchCombLinearStScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 120), PitchDimension::get())
{
}

PitchCombLinearStScaleConverter::~PitchCombLinearStScaleConverter()
{
}
