#include "FineBipolar200StScaleConverter.h"
#include "parameters/scale-converters/dimension/PitchDimension.h"

FineBipolar200StScaleConverter::FineBipolar200StScaleConverter()
: LinearScaleConverter(tDisplayRange(-200, 200), PitchDimension::get())
{
}

FineBipolar200StScaleConverter::~FineBipolar200StScaleConverter() noexcept = default;