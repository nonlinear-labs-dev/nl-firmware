#include "Linear25DbScaleConverter.h"
#include "dimension/LevelDimension.h"

Linear25DbScaleConverter::Linear25DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 25), LevelDimension::get())
{
}

Linear25DbScaleConverter::~Linear25DbScaleConverter()
{
}
