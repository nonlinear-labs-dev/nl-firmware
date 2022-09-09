#include "LinearBipolar36StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar36StScaleConverter::LinearBipolar36StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-36, 36), PitchDimensionCoarse::get())
{
}

LinearBipolar36StScaleConverter::~LinearBipolar36StScaleConverter()
{
}

LinearBipolar72StScaleConverter::LinearBipolar72StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-72, 72), PitchDimensionCoarse::get())
{
}
