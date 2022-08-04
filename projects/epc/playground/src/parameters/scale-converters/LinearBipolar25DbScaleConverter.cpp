#include "dimension/LevelDimension.h"
#include "LinearBipolar25DbScaleConverter.h"

LinearBipolar25DbScaleConverter::LinearBipolar25DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-25, 25), LevelDimension::get())
{
}

LinearBipolar25DbScaleConverter::~LinearBipolar25DbScaleConverter()
{
}
