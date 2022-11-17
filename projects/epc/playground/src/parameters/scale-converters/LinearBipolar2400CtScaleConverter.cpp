#include "LinearBipolar2400CtScaleConverter.h"
#include "parameters/scale-converters/dimension/CentDimension.h"

LinearBipolar2400CtScaleConverter::LinearBipolar2400CtScaleConverter()
    : LinearScaleConverter(tDisplayRange(-2400, 2400), CentDimension::get())
{
}
