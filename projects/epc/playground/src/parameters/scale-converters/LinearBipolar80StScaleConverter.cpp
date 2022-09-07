#include "LinearBipolar80StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar80StScaleConverter::LinearBipolar80StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-80, 80), PitchDimensionCoarse::get())
{
}

LinearBipolar80StScaleConverter::~LinearBipolar80StScaleConverter()
{
}
