#include "Linear24StScaleConverter.h"
#include "dimension/PitchDimension.h"

Linear24StScaleConverter::Linear24StScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 24), PitchDimension::get())
{
}

Linear24StScaleConverter::~Linear24StScaleConverter()
{
}
