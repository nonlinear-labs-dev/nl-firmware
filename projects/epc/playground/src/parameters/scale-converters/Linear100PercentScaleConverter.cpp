#include "Linear100PercentScaleConverter.h"
#include "dimension/PercentageDimension.h"

Linear100PercentScaleConverter::Linear100PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 100), PercentageDimension::get())
{
}

Linear100PercentScaleConverter::~Linear100PercentScaleConverter() noexcept = default;

Linear200PercentScaleConverter::Linear200PercentScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 200), PercentageDimension::get())
{
}

Linear200PercentScaleConverter::~Linear200PercentScaleConverter() noexcept = default;