#include <parameters/scale-converters/dimension/DBTDimension.h>
#include "LinearBipolarInverted60DBTScaleConverter.h"

LinearBipolarInverted60DbtScaleConverter::LinearBipolarInverted60DbtScaleConverter()
    : LinearScaleConverter(tTcdRange(-60, 60), tDisplayRange(-60, 60), DBTDimension::get())

{
}

LinearBipolarInverted60DbtScaleConverter::~LinearBipolarInverted60DbtScaleConverter() = default;
