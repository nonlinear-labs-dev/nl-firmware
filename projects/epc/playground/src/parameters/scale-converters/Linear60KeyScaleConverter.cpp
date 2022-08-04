#include "Linear60KeyScaleConverter.h"
#include "dimension/KeyDimension.h"

Linear60KeyScaleConverter::Linear60KeyScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 60), KeyDimension::get())
{
}
