#include "Linear96StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

Linear96StScaleConverter::Linear96StScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 96), PitchDimensionCoarse::get())
{
}

Linear96StScaleConverter::~Linear96StScaleConverter()
{
}
