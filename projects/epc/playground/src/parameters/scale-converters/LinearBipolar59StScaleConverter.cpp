#include "LinearBipolar59StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar59StScaleConverter::LinearBipolar59StScaleConverter()
    : LinearScaleConverter(tTcdRange(-6000, 6000), tDisplayRange(-59, 59), PitchDimensionCoarse::get())
{
}
