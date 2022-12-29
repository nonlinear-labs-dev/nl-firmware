#include "SplitPointDimension.h"

Glib::ustring SplitPointDimension::stringize(const tDisplayValue &displayValue) const
{
  return KeyDimension::stringizeNote(round(displayValue));
}

Glib::ustring SplitPointDimension::getStingizerJS() const
{
  return "formatNote(Math.round(cpValue))";
}

const SplitPointDimension &SplitPointDimension::get()
{
  static SplitPointDimension dim;
  return dim;
}
