#include "SplitPointDimension.h"

Glib::ustring SplitPointDimension::stringize(const tDisplayValue &displayValue) const
{
  return KeyDimension::stringizeNote((int) displayValue);
}

const SplitPointDimension &SplitPointDimension::get()
{
  static SplitPointDimension dim;
  return dim;
}
