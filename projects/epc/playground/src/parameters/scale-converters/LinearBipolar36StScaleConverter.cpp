#include "LinearBipolar36StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar36StScaleConverter::LinearBipolar36StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-36, 36), PitchDimensionCoarse::get())
{
}

LinearBipolar36StScaleConverter::~LinearBipolar36StScaleConverter()
{
}
