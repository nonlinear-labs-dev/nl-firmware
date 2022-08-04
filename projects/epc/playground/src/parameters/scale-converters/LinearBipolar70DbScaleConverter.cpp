#include <parameters/scale-converters/dimension/LevelDimension.h>
#include "LinearBipolar70DbScaleConverter.h"

LinearBipolar70DbScaleConverter::LinearBipolar70DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-70, 70), LevelDimension::get())
{
}
