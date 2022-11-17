#include "FineBipolar100PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

FineBipolar100PercentScaleConverter::FineBipolar100PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-100, 100), PercentageDimensionFine::get())
{
}

FineBipolar100PercentScaleConverter::~FineBipolar100PercentScaleConverter()
{
}
