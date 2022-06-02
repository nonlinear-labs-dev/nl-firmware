#include "FineBipolar1200CTScaleConverter.h"
#include "parameters/scale-converters/dimension/CentDimension.h"

FineBipolar1200CTScaleConverter::FineBipolar1200CTScaleConverter()
    : LinearScaleConverter(tTcdRange(-1200, 1200), tDisplayRange(-1200, 1200), CentDimension::get())
{
}
