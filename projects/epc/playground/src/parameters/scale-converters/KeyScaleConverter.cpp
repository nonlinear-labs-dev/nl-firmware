#include "KeyScaleConverter.h"
#include "dimension/CentDimension.h"

static const short c_range = 1200;

KeyScaleConverter::KeyScaleConverter()
    : LinearScaleConverter(tDisplayRange(-c_range, c_range), CentDimension::get())
{
}

KeyScaleConverter::~KeyScaleConverter()
{
}
