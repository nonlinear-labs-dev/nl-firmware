#include "PitchOscLinearStScaleModulationConverter.h"
#include "dimension/PitchDimension.h"

PitchOscLinearStScaleModulationConverter::PitchOscLinearStScaleModulationConverter()
    : LinearScaleConverter(tDisplayRange(-150, 150), PitchDimension::get())
{
}

PitchOscLinearStScaleModulationConverter::~PitchOscLinearStScaleModulationConverter()
{
}
