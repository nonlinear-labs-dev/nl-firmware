#include "LinearBipolar200PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar200PercentScaleConverter::LinearBipolar200PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimension::get())
{
}

LinearBipolar200PercentScaleConverter::~LinearBipolar200PercentScaleConverter()
{
}
