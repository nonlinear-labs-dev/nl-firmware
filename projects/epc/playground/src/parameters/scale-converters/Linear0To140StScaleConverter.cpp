#include "Linear0To140StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

Linear0To140StScaleConverter::Linear0To140StScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 140), PitchDimensionCoarse::get())
{
}

Linear0To140StScaleConverter::~Linear0To140StScaleConverter()
{
}
