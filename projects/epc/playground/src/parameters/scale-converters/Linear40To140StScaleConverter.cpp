#include <parameters/scale-converters/dimension/PitchDimensionCoarse.h>
#include "Linear40To140StScaleConverter.h"

Linear40To140StScaleConverter::Linear40To140StScaleConverter()
    : LinearScaleConverter(tDisplayRange(40, 140), PitchDimensionCoarse::get())
{
}

Linear40To140StScaleConverter::~Linear40To140StScaleConverter()
{
}
