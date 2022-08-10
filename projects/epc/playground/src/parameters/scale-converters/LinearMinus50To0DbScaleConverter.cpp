#include "LinearMinus50To0DbScaleConverter.h"
#include "dimension/LevelDimension.h"

LinearMinus50To0DbScaleConverter::LinearMinus50To0DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-50, 0), LevelDimension::get())
{
}

LinearMinus50To0DbScaleConverter::~LinearMinus50To0DbScaleConverter()
{
}
