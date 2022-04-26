#include "Fine105PercentScaleConverter.h"
#include "dimension/PercentageDimensionFine.h"

Fine105PercentScaleConverter::Fine105PercentScaleConverter()
    : LinearScaleConverter(tTcdRange(0, 10500), tDisplayRange(0, 105), PercentageDimensionFine::get())
{
}

Fine105PercentScaleConverter::~Fine105PercentScaleConverter() noexcept = default;

Fine200PercentScaleConverter::Fine200PercentScaleConverter()
  : LinearScaleConverter(tTcdRange(0, 16000), tDisplayRange(0, 200), PercentageDimensionFine::get())
{
}

Fine200PercentScaleConverter::~Fine200PercentScaleConverter() noexcept = default;