#pragma once

#include "Dimension.h"

template <int tPrecision = 3> class TimeDimension : public Dimension
{
 public:
  Glib::ustring getStingizerJS() const override
  {
    return "formatDimensionDigits(cpValue, 'ms', withUnit, " + std::to_string(tPrecision) + ")";
  }

  Glib::ustring stringize(const tDisplayValue &displayValue) const override
  {
    return formatDimensionDigits(displayValue, "ms", tPrecision);
  }

  static const TimeDimension &get()
  {
    static TimeDimension<tPrecision> p;
    return p;
  }
};
