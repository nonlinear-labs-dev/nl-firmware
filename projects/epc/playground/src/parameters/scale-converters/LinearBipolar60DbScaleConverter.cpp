#include <parameters/scale-converters/dimension/LevelDimension.h>
#include "LinearBipolar60DbScaleConverter.h"

LinearBipolar60DbScaleConverter::LinearBipolar60DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-60, 60), LevelDimension::get())
{
}

LinearBipolar60DbScaleConverter::~LinearBipolar60DbScaleConverter() = default;
