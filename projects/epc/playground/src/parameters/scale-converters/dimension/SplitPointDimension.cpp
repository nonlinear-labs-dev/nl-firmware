#include "SplitPointDimension.h"

Glib::ustring SplitPointDimension::stringize(const tDisplayValue &displayValue) const
{
  int note = std::round(displayValue * 59);
  return KeyDimension::stringizeNote(note);
}

const SplitPointDimension &SplitPointDimension::get()
{
  static SplitPointDimension dim;
  return dim;
}
