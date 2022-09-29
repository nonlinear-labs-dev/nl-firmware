#include "LinearBipolar200PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

LinearBipolar200PercentScaleConverter::LinearBipolar200PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimension::get())
{
}

LinearBipolar200PercentScaleConverter::~LinearBipolar200PercentScaleConverter()
{
}

tControlPositionValue LinearBipolar200PercentScaleConverter::getCoarseDenominator(const QuantizedValue &v) const
{
  return ScaleConverter::getCoarseDenominator(v) * 2;
}

tControlPositionValue LinearBipolar200PercentScaleConverter::getFineDenominator(const QuantizedValue &v) const
{
  return ScaleConverter::getFineDenominator(v) * 2;
}

LinearBipolar200PercentFineScaleConverter::LinearBipolar200PercentFineScaleConverter()
    : LinearScaleConverter(tDisplayRange(-200, 200), PercentageDimension::get())
{
}

LinearBipolar200PercentFineScaleConverter::~LinearBipolar200PercentFineScaleConverter()
{
}

tControlPositionValue LinearBipolar200PercentFineScaleConverter::getCoarseDenominator(const QuantizedValue &v) const
{
  return ScaleConverter::getCoarseDenominator(v) * 2;
}

tControlPositionValue LinearBipolar200PercentFineScaleConverter::getFineDenominator(const QuantizedValue &v) const
{
  return ScaleConverter::getFineDenominator(v) * 2;
}
