#include "FrequencyDimension.h"

FrequencyDimension::FrequencyDimension() = default;

FrequencyDimension::~FrequencyDimension() = default;

const FrequencyDimension &FrequencyDimension::get()
{
  static FrequencyDimension d;
  return d;
}

Glib::ustring FrequencyDimension::getStingizerJS() const
{
  return "formatDimensionDigits(cpValue, 'Hz', withUnit, 3)";
}

Glib::ustring FrequencyDimension::stringize(const tDisplayValue &displayValue) const
{
  return formatDimensionDigits(displayValue, "Hz", 4);
}
