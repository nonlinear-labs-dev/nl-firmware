#include "LevelScaleConverter.h"
#include "dimension/LevelDimension.h"

LevelScaleConverter::LevelScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 100), LevelDimension::get())
{
}

LevelScaleConverter::~LevelScaleConverter()
{
}
