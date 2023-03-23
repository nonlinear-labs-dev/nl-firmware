#include "TuneReferenceFrequencyDimension.h"

const TuneReferenceFrequencyDimension &TuneReferenceFrequencyDimension::get()
{
  static TuneReferenceFrequencyDimension d;
  return d;
}

Glib::ustring TuneReferenceFrequencyDimension::getStingizerJS() const
{
  return "formatDimensionDigits(cpValue, 'Hz', withUnit, 4)";
}

Glib::ustring TuneReferenceFrequencyDimension::stringize(const tDisplayValue &displayValue) const
{
  return formatDimensionDigits(displayValue, "Hz", 4);
}
