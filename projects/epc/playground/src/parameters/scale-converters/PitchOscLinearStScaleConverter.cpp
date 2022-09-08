#include "PitchOscLinearStScaleConverter.h"
#include "dimension/PitchDimension.h"

PitchOscLinearStScaleConverter::PitchOscLinearStScaleConverter()
    : LinearScaleConverter(tDisplayRange(-20, 130), PitchDimension::get())
{
}

PitchOscLinearStScaleConverter::~PitchOscLinearStScaleConverter()
{
}
