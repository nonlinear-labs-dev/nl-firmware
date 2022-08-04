#include "Linear180DegreeScaleConverter.h"
#include "dimension/DegreeDimension.h"

Linear180DegreeScaleConverter::Linear180DegreeScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 180), DegreeDimension::get())
{
}

Linear180DegreeScaleConverter::~Linear180DegreeScaleConverter()
{
}
