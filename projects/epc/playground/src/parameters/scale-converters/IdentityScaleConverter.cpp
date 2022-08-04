#include "IdentityScaleConverter.h"
#include "dimension/UnitlessDimension.h"

IdentityScaleConverter::IdentityScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 100), UnitlessDimension::get())
{
}

IdentityScaleConverter::~IdentityScaleConverter()
{
}
