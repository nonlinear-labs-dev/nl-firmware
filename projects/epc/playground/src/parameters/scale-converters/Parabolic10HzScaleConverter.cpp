#include "Parabolic10HzScaleConverter.h"
#include "dimension/FrequencyDimension.h"

Parabolic10HzScaleConverter::Parabolic10HzScaleConverter()
    : ParabolicScaleConverter(tDisplayRange(0, 10), FrequencyDimension::get())
{
}

Parabolic10HzScaleConverter::~Parabolic10HzScaleConverter()
{
}
