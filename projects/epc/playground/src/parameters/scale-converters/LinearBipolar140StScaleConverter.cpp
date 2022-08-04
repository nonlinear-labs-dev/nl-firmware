#include "LinearBipolar140StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar140StScaleConverter::LinearBipolar140StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-140, 140), PitchDimensionCoarse::get())
{
}

LinearBipolar140StScaleConverter::~LinearBipolar140StScaleConverter()
{
}
