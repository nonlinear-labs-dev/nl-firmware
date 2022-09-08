#include "dimension/LevelDimension.h"
#include "LinearBipolar50DbScaleConverter.h"

LinearBipolar50DbScaleConverter::LinearBipolar50DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-50, 50), LevelDimension::get())
{
}

LinearBipolar50DbScaleConverter::~LinearBipolar50DbScaleConverter()
{
}
