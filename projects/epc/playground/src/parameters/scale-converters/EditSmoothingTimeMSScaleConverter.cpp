#include "EditSmoothingTimeMSScaleConverter.h"
#include "dimension/TimeDimension.h"

EditSmoothingTimeMSScaleConverter::EditSmoothingTimeMSScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 2000.0), TimeDimension<3>::get())
{
}
