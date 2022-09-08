#include "LinearBipolar100StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar100StScaleConverter::LinearBipolar100StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-100, 100), PitchDimensionCoarse::get())
{
}

LinearBipolar100StScaleConverter::~LinearBipolar100StScaleConverter()
{
}
