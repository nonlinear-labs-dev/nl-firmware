#include "dimension/LevelDimension.h"
#include "LinearBipolar24DbScaleConverter.h"

LinearBipolar24DbScaleConverter::LinearBipolar24DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-24, 24), LevelDimension::get())
{
}

LinearBipolar24DbScaleConverter::~LinearBipolar24DbScaleConverter()
{
}
