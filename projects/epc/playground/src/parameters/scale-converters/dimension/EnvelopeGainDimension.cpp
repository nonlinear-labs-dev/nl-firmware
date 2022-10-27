#include "EnvelopeGainDimension.h"

EnvelopeGainDimension::EnvelopeGainDimension()
{
}

EnvelopeGainDimension::~EnvelopeGainDimension()
{
}

const EnvelopeGainDimension &EnvelopeGainDimension::get()
{
  static EnvelopeGainDimension d;
  return d;
}

Glib::ustring EnvelopeGainDimension::getStingizerJS() const
{
  return "(formatDimensionRounded(cpValue, 'dB', withUnit, 0.01))";
}

Glib::ustring EnvelopeGainDimension::stringize(const tDisplayValue &displayValue) const
{
  return formatDimensionRounded(displayValue, "dB", 0.01);
}
