#include <parameters/scale-converters/Parabolic2000MsScaleConverter.h>
#include "dimension/TimeDimension.h"

Parabolic2000MsScaleConverter::Parabolic2000MsScaleConverter()
    : ParabolicScaleConverter(tDisplayRange(0, 2000), TimeDimension<3>::get())
{
}

Parabolic2000MsScaleConverter::~Parabolic2000MsScaleConverter()
{
}
