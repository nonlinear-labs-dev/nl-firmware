#include "ParabolicScaleConverter.h"
#include "dimension/PercentageDimension3Digits.h"

ParabolicScaleConverter::ParabolicScaleConverter(tDisplayRange displayRange, const Dimension& dimension)
    : ScaleConverter(dimension)
    , m_displayRange(displayRange)
{
}

ParabolicScaleConverter::~ParabolicScaleConverter()
{
}

bool ParabolicScaleConverter::isBiPolar() const
{
  return false;
}

tDisplayValue ParabolicScaleConverter::controlPositionToDisplay(const tControlPositionValue &cpValue) const
{
  tControlPositionValue in = getControlPositionRange().clip(cpValue);
  return (in * in) * m_displayRange.getRangeWidth() + m_displayRange.getMin();
}

Glib::ustring ParabolicScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "return $wnd.formatDimension(cpValue * cpValue * " << m_displayRange.getRangeWidth() << " + "
    << m_displayRange.getMin() << ", \"" << getDimension().getStingizerJS() << "\", withUnit);";
  return s.str();
}
