#include "FineBipolar160StScaleConverter.h"
#include "parameters/scale-converters/dimension/PitchDimension.h"

FineBipolar160StScaleConverter::FineBipolar160StScaleConverter()
: LinearScaleConverter(tTcdRange(-8000, 8000), tDisplayRange(-160, 160), PitchDimension::get())
{
}

FineBipolar160StScaleConverter::~FineBipolar160StScaleConverter() noexcept = default;