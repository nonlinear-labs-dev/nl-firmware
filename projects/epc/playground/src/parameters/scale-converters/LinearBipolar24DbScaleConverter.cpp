#include "dimension/EnvelopeGainDimension.h"
#include "LinearBipolar24DbScaleConverter.h"

LinearBipolar24DbScaleConverter::LinearBipolar24DbScaleConverter()
    : LinearScaleConverter(tDisplayRange(-24, 24), EnvelopeGainDimension::get())
{
}

LinearBipolar24DbScaleConverter::~LinearBipolar24DbScaleConverter()
{
}
