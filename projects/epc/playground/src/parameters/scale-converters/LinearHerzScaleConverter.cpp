#include <parameters/scale-converters/dimension/FrequencyDimension.h>
#include "LinearHerzScaleConverter.h"

LinearHerzScaleConverter::LinearHerzScaleConverter()
    : LinearScaleConverter(tDisplayRange { 400, 480 }, FrequencyDimension::get())
{
}
