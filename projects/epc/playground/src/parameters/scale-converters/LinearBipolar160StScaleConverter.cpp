#include "LinearBipolar160StScaleConverter.h"

LinearBipolar160StScaleConverter::LinearBipolar160StScaleConverter()
    : LinearScaleConverter(tTcdRange(-7500, 7500), tDisplayRange(-160, 160), PitchDimensionCoarse::get())
{
}

LinearBipolar160StScaleConverter::~LinearBipolar160StScaleConverter()
{
}
