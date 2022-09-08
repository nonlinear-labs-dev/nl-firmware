#include "Linear60To140StScaleConverter.h"
#include "dimension/PitchDimensionCoarse.h"

Linear60To140StScaleConverter::Linear60To140StScaleConverter()
    : LinearScaleConverter(tDisplayRange(60, 140), PitchDimensionCoarse::get())
{
}

Linear60To140StScaleConverter::~Linear60To140StScaleConverter()
{
}
