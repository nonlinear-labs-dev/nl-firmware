#include <parameters/scale-converters/dimension/PitchDimension.h>
#include "LinearBipolar96StScaleConverterFine.h"

LinearBipolar96StScaleConverterFine::LinearBipolar96StScaleConverterFine()
    : LinearScaleConverter(tDisplayRange(-96, 96), PitchDimension::get())
{
}
