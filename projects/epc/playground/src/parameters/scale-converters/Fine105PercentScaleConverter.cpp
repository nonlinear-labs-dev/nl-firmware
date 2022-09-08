#include "Fine105PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

Fine105PercentScaleConverter::Fine105PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 105), PercentageDimensionFine::get())
{
}

Fine105PercentScaleConverter::~Fine105PercentScaleConverter() noexcept = default;

Fine200PercentScaleConverter::Fine200PercentScaleConverter()
  : LinearScaleConverter(tDisplayRange(0, 200), PercentageDimensionFine::get())
{
}

Fine200PercentScaleConverter::~Fine200PercentScaleConverter() noexcept = default;