#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include "SplitPointScaleConverter.h"

SplitPointScaleConverter::SplitPointScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 60), SplitPointDimension::get())
{
}
