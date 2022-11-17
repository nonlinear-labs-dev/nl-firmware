#include "FineBipolar200PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

FineBipolar200PercentScaleConverter::FineBipolar200PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimensionFine::get())
{
}

FineBipolar200PercentScaleConverter::~FineBipolar200PercentScaleConverter()
{
}
