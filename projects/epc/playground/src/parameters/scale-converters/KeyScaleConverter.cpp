#include "KeyScaleConverter.h"
#include "dimension/CentDimension.h"

static const short c_range = 1200;

KeyScaleConverter::KeyScaleConverter()
    : LinearScaleConverter(tTcdRange(-10 * c_range, 10 * c_range), tDisplayRange(-c_range, c_range),
                           CentDimension::get())
{
}

KeyScaleConverter::~KeyScaleConverter()
{
}
