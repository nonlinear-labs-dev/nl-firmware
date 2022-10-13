#include "FineBipolar33PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

FineBipolar33PercentScaleConverter::FineBipolar33PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-33, 33), PercentageDimensionFine::get())
{
}

FineBipolar33PercentScaleConverter::~FineBipolar33PercentScaleConverter()
{
}
