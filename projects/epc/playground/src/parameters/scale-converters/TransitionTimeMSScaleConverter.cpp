#include "TransitionTimeMSScaleConverter.h"

TransitionTimeMSScaleConverter::TransitionTimeMSScaleConverter()
    : TimeScaleConverter(tDisplayRange(0.1, 16000.0))
{
}

TransitionTimeMSScaleConverter::~TransitionTimeMSScaleConverter()
{
}
