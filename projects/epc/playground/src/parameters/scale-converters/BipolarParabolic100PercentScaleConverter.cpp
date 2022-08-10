#include <parameters/scale-converters/BipolarParabolic100PercentScaleConverter.h>
#include "dimension/PercentageDimension3Digits.h"

BipolarParabolic100PercentScaleConverter::BipolarParabolic100PercentScaleConverter()
    : ScaleConverter(PercentageDimension3Digits::get())
    , m_displayRange(-100, 100)
{
}

BipolarParabolic100PercentScaleConverter::~BipolarParabolic100PercentScaleConverter()
{
}

bool BipolarParabolic100PercentScaleConverter::isBiPolar() const
{
  return true;
}

tDisplayValue
    BipolarParabolic100PercentScaleConverter::controlPositionToDisplay(const tControlPositionValue &cpValue) const
{
  tControlPositionValue in = getControlPositionRange().clip(cpValue);
  return (in * std::abs(in)) * 100;
}

Glib::ustring BipolarParabolic100PercentScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "return $wnd.formatDimension(cpValue * Math.abs(cpValue) * 100, \"" << getDimension().getStingizerJS()
    << "\", withUnit);";
  return s.str();
}