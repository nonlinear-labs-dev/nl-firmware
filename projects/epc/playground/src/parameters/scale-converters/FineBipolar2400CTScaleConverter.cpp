#include "FineBipolar2400CTScaleConverter.h"
#include "parameters/scale-converters/dimension/CentDimension.h"

FineBipolar2400CTScaleConverter::FineBipolar2400CTScaleConverter()
    : LinearScaleConverter(tTcdRange(-1200, 1200), tDisplayRange(-1200, 1200), CentDimension::get())
{
}
