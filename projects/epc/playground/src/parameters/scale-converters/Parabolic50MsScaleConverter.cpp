#include <parameters/scale-converters/Parabolic50MsScaleConverter.h>
#include "dimension/TimeDimension.h"

Parabolic50MsScaleConverter::Parabolic50MsScaleConverter()
    : ParabolicScaleConverter(tDisplayRange(0, 50), TimeDimension<3>::get())
{
}
