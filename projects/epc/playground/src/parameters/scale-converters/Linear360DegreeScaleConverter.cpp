#include "Linear360DegreeScaleConverter.h"
#include "dimension/DegreeDimension.h"

Linear360DegreeScaleConverter::Linear360DegreeScaleConverter()
    : super(tDisplayRange(0, 360), DegreeDimension::get())
{
}

Linear360DegreeScaleConverter::~Linear360DegreeScaleConverter()
{
}
