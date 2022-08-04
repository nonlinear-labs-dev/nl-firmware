#include <parameters/scale-converters/dimension/PitchDimensionCoarse.h>
#include "Linear60StScaleConverter.h"

Linear60StScaleConverter::Linear60StScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 60), PitchDimensionCoarse::get())
{
}
