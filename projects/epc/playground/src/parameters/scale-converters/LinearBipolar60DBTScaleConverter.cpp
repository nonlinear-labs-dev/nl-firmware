#include <parameters/scale-converters/dimension/DBTDimension.h>
#include "LinearBipolar60DBTScaleConverter.h"

LinearBipolar60DbtScaleConverter::LinearBipolar60DbtScaleConverter()
    : LinearScaleConverter(tTcdRange(-60, 60), tDisplayRange(-60, 60), DBTDimension::get())

{
}

LinearBipolar60DbtScaleConverter::~LinearBipolar60DbtScaleConverter() = default;
