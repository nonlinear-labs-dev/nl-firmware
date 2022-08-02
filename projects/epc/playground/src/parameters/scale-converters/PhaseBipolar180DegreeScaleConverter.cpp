#include "PhaseBipolar180DegreeScaleConverter.h"
#include "dimension/DegreeDimension.h"

PhaseBipolar180DegreeScaleConverter::PhaseBipolar180DegreeScaleConverter()
    : LinearScaleConverter(tDisplayRange(-180, 180), DegreeDimension::get())
{
}

PhaseBipolar180DegreeScaleConverter::~PhaseBipolar180DegreeScaleConverter()
{
}
