#include "FineBipolar66PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

FineBipolar66PercentScaleConverter::FineBipolar66PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-66, 66), PercentageDimensionFine::get())
{
}

FineBipolar66PercentScaleConverter::~FineBipolar66PercentScaleConverter()
{
}
