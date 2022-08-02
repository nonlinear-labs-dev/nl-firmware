#include "LinearBipolar150StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar150StScaleConverter::LinearBipolar150StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-150, 150), PitchDimensionCoarse::get())
{
}

LinearBipolar150StScaleConverter::~LinearBipolar150StScaleConverter()
{
}
