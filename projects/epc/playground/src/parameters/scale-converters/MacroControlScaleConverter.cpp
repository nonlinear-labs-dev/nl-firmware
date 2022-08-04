#include "MacroControlScaleConverter.h"
#include "dimension/PercentageDimension.h"

MacroControlScaleConverter::MacroControlScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 100), PercentageDimension::get())
{
}
