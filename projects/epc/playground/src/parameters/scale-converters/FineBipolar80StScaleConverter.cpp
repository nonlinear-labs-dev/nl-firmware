#include "FineBipolar80StScaleConverter.h"
#include "dimension/PitchDimension.h"

FineBipolar80StScaleConverter::FineBipolar80StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-80, 80), PitchDimension::get())
{
}

FineBipolar80StScaleConverter::~FineBipolar80StScaleConverter()
{
}
