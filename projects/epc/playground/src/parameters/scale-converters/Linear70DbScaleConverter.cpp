#include <parameters/scale-converters/dimension/LevelDimension.h>
#include "Linear70DbScaleConverter.h"

Linear70DbScaleConverter::Linear70DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 70), LevelDimension::get())
{
}