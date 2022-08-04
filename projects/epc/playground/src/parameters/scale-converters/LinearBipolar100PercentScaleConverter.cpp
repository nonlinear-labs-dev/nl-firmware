#include "LinearBipolar100PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar100PercentScaleConverter::LinearBipolar100PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-100, 100), PercentageDimension::get())
{
}

LinearBipolar100PercentScaleConverter::~LinearBipolar100PercentScaleConverter()
{
}
