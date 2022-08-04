#include "Linear50DbScaleConverter.h"
#include "dimension/LevelDimension.h"

Linear50DbScaleConverter::Linear50DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 50), LevelDimension::get())
{
}

Linear50DbScaleConverter::~Linear50DbScaleConverter()
{
}
