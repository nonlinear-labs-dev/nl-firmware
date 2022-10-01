#include "FineBipolar72StScaleConverter.h"
#include "dimension/PitchDimension.h"

FineBipolar72StScaleConverter::FineBipolar72StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-72, 72), PitchDimension::get())
{
}
