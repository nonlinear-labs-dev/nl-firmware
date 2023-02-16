#include "LinearScaleConverter.h"

LinearScaleConverter::LinearScaleConverter(const tDisplayRange& displayRange, const Dimension& dim)
    : ScaleConverter(dim)
    , m_displayRange(displayRange)
{
}

LinearScaleConverter::~LinearScaleConverter()
{
}

bool LinearScaleConverter::isBiPolar() const
{
  return m_displayRange.isBiPolar();
}

tDisplayValue LinearScaleConverter::controlPositionToDisplay(const tControlPositionValue& cpValue) const
{
  return m_displayRange.scaleValueToRange(cpValue, getControlPositionRange(), true);
}

Glib::ustring LinearScaleConverter::controlPositionToDisplayJS() const
{
  return m_displayRange.getScaleValueToRangeJS(getControlPositionRange(), getDimension());
}

size_t LinearScaleConverter::hash() const
{
  return super::hash() ^ (m_displayRange.hash() << 1);
}