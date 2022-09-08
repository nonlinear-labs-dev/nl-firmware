#include "LinearBipolar33PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar33PercentScaleConverter::LinearBipolar33PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-33, 33), PercentageDimension::get())
{
}

LinearBipolar33PercentScaleConverter::~LinearBipolar33PercentScaleConverter()
{
}
