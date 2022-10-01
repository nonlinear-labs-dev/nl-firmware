#include "LinearBipolar200StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar200StScaleConverter::LinearBipolar200StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PitchDimensionCoarse::get())
{
}

LinearBipolar200StScaleConverter::~LinearBipolar200StScaleConverter()
{
}
