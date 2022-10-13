#include "dimension/EnvelopeGainDimension.h"
#include "LinearBipolar48DbScaleConverter.h"

LinearBipolar48DbScaleConverter::LinearBipolar48DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-48, 48), EnvelopeGainDimension::get())
{
}

LinearBipolar48DbScaleConverter::~LinearBipolar48DbScaleConverter()
{
}
