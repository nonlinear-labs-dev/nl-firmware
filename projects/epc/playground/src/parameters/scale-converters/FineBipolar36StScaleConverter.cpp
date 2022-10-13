#include "FineBipolar36StScaleConverter.h"
#include "dimension/PitchDimension.h"

FineBipolar36StScaleConverter::FineBipolar36StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-36, 36), PitchDimension::get())
{
}

FineBipolar36StScaleConverter::~FineBipolar36StScaleConverter()
{
}
