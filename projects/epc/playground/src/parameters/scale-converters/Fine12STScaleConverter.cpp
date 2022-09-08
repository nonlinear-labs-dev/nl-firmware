#include "Fine12STScaleConverter.h"
#include "dimension/PitchDimensionFine.h"

Fine12STScaleConverter::Fine12STScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 12), PitchDimensionFine::get())
{
}

Fine12STScaleConverter::~Fine12STScaleConverter()
{
}

Fine24STScaleConverter::Fine24STScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 24), PitchDimensionFine::get())
{
}
