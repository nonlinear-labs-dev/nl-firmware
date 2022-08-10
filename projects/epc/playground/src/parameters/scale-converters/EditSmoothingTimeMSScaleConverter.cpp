#include "EditSmoothingTimeMSScaleConverter.h"
#include "dimension/TimeDimension.h"

EditSmoothingTimeMSScaleConverter::EditSmoothingTimeMSScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 200.0), TimeDimension<3>::get())
{
}
