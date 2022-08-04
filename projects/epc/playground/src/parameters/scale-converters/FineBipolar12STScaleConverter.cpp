#include "FineBipolar12STScaleConverter.h"
#include "dimension/PitchDimensionFine.h"

FineBipolar12STScaleConverter::FineBipolar12STScaleConverter()
    : LinearScaleConverter(tDisplayRange(-12, 12), PitchDimensionFine::get())
{
}

FineBipolar12STScaleConverter::~FineBipolar12STScaleConverter()
{
}

FineBipolar24STScaleConverter::FineBipolar24STScaleConverter()
    : LinearScaleConverter(tDisplayRange(-24, 24), PitchDimensionFine::get())
{
}