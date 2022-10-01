#include "LinearBipolar1200CtScaleConverter.h"
#include "dimension/CentDimension.h"

LinearBipolar1200CtScaleConverter::LinearBipolar1200CtScaleConverter()
    : LinearScaleConverter(tDisplayRange(-1200, 1200), CentDimension::get())
{
}

LinearBipolar1200CtScaleConverter::~LinearBipolar1200CtScaleConverter()
{
}
