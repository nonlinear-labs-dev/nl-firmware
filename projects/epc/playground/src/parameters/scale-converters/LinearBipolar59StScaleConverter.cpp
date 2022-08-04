#include "LinearBipolar59StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar59StScaleConverter::LinearBipolar59StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-59, 59), PitchDimensionCoarse::get())
{
}
