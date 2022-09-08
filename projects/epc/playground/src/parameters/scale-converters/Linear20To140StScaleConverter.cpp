#include "Linear20To140StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

Linear20To140StScaleConverter::Linear20To140StScaleConverter()
    : LinearScaleConverter(tDisplayRange(20, 140), PitchDimensionCoarse::get())
{
}

Linear20To140StScaleConverter::~Linear20To140StScaleConverter()
{
}
