#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include "SplitPointScaleConverter.h"

SplitPointScaleConverter::SplitPointScaleConverter()
    : LinearScaleConverter(tTcdRange(0, 59), tDisplayRange(0, 59), SplitPointDimension::get())
{
}
