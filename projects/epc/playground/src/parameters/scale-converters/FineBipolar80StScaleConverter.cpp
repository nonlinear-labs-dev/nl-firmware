#include "FineBipolar80StScaleConverter.h"
#include "dimension/PitchDimension.h"

FineBipolar80StScaleConverter::FineBipolar80StScaleConverter()
    : LinearScaleConverter(tTcdRange(-8000, 8000), tDisplayRange(-80, 80), PitchDimension::get())
{
}

FineBipolar80StScaleConverter::~FineBipolar80StScaleConverter()
{
}

FineBipolar160StScaleConverter::FineBipolar160StScaleConverter()
    : LinearScaleConverter(tTcdRange(-8000, 8000), tDisplayRange(-160, 160), PitchDimension::get())
{
}
