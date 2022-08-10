#include <parameters/scale-converters/dimension/DBTDimension.h>
#include "LinearBipolarInverted60DBTScaleConverter.h"

LinearBipolarInverted60DbtScaleConverter::LinearBipolarInverted60DbtScaleConverter()
    : LinearScaleConverter(tDisplayRange(-60, 60), DBTDimension::get())

{
}

LinearBipolarInverted60DbtScaleConverter::~LinearBipolarInverted60DbtScaleConverter() = default;
