#include <parameters/scale-converters/dimension/TuneReferenceFrequencyDimension.h>
#include "LinearHerzScaleConverter.h"

LinearHerzScaleConverter::LinearHerzScaleConverter()
    : LinearScaleConverter(tDisplayRange { 400, 480 }, TuneReferenceFrequencyDimension::get())
{
}
