#include "LinearBipolar66PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar66PercentScaleConverter::LinearBipolar66PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-66, 66), PercentageDimension::get())
{
}

LinearBipolar66PercentScaleConverter::~LinearBipolar66PercentScaleConverter()
{
}
