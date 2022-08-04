#include "LinearBipolar96StScaleConverterCoarse.h"
#include "dimension/PitchDimensionCoarse.h"

LinearBipolar96StScaleConverterCoarse::LinearBipolar96StScaleConverterCoarse()
    : LinearScaleConverter(tDisplayRange(-96, 96), PitchDimensionCoarse::get())
{
}

LinearBipolar96StScaleConverterCoarse::~LinearBipolar96StScaleConverterCoarse()
{
}
