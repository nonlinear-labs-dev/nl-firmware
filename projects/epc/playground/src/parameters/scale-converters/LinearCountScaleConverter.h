#pragma once
#include "LinearScaleConverter.h"

template <int tCount, typename tDimension> class LinearCountScaleConverter : public LinearScaleConverter
{
 public:
  LinearCountScaleConverter()
      : LinearScaleConverter(tDisplayRange(0, tCount - 1), tDimension::get())
  {
  }
};