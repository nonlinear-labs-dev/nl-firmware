#include "LinearBipolar200PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"
#include "dimension/PercentageDimensionFine.h"

LinearBipolar200PercentScaleConverter::LinearBipolar200PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimension::get())
{
}

LinearBipolar200PercentScaleConverter::~LinearBipolar200PercentScaleConverter()
{
}

LinearBipolar200PercentFineScaleConverter::LinearBipolar200PercentFineScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimensionFine::get())
{
}

LinearBipolar200PercentFineScaleConverter::~LinearBipolar200PercentFineScaleConverter()
{
}
