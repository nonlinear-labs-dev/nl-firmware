#include "dimension/DBTDimension.h"
#include "Linear60DbtScaleConverter.h"

Linear60DbtScaleConverter::Linear60DbtScaleConverter()
    : LinearScaleConverter(tTcdRange(0, 12000), tDisplayRange(-60, 0), DBTDimension::get())
{
}

Linear60DbtScaleConverter::~Linear60DbtScaleConverter()
{
}
