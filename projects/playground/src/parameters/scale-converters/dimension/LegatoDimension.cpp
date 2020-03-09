#include "LegatoDimension.h"

Glib::ustring LegatoDimension::stringize(const tDisplayValue& displayValue) const
{
  switch(static_cast<int>(displayValue))
  {
    default:
    case 0:
      return "No Effect";
    case 1:
      return "On Envelopes";
    case 2:
      return "Glide";
    case 3:
      return "On Env & Glide";
  }
}

LegatoDimension LegatoDimension::get()
{
  static LegatoDimension ret;
  return ret;
}
