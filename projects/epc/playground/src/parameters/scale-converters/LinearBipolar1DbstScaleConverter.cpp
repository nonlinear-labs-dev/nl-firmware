#include "dimension/DBPerSTDimension.h"
#include "LinearBipolar1DbstScaleConverter.h"

LinearBipolar1DbstScaleConverter::LinearBipolar1DbstScaleConverter()
    : LinearScaleConverter(tDisplayRange(-1, 1), DBPerSTDimension::get())
{
}

LinearBipolar1DbstScaleConverter::~LinearBipolar1DbstScaleConverter()
{
}
