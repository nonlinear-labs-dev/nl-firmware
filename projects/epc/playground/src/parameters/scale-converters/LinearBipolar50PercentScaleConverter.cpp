#include "LinearBipolar50PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar50PercentScaleConverter::LinearBipolar50PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-50, 50), PercentageDimension::get())
{
}

LinearBipolar50PercentScaleConverter::~LinearBipolar50PercentScaleConverter()
{
}
