#include "dimension/LevelDimension.h"
#include "LinearBipolar48DbScaleConverter.h"

LinearBipolar48DbScaleConverter::LinearBipolar48DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-48, 48), LevelDimension::get())
{
}

LinearBipolar48DbScaleConverter::~LinearBipolar48DbScaleConverter()
{
}
