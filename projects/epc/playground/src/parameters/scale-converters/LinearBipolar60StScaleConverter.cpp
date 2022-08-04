#include "LinearBipolar60StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar60StScaleConverter::LinearBipolar60StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-60, 60), PitchDimensionCoarse::get())
{
}
