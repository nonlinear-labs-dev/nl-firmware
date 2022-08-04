#include "LinearBipolar48StScaleConverter.h"
#include "dimension/PitchDimension.h"

LinearBipolar48StScaleConverter::LinearBipolar48StScaleConverter()
    : LinearScaleConverter(tDisplayRange(-48, 48), PitchDimension::get())
{
}

LinearBipolar48StScaleConverter::~LinearBipolar48StScaleConverter()
{
}
