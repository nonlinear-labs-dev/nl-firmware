#include "LinearBipolar160StScaleConverter.h"

LinearBipolar160StScaleConverter::LinearBipolar160StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-160, 160), PitchDimensionCoarse::get())
{
}

LinearBipolar160StScaleConverter::~LinearBipolar160StScaleConverter()
{
}
