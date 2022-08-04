#include <parameters/scale-converters/dimension/NoteDimension.h>
#include "BaseKeyScaleConverter.h"
#include "dimension/NoteDimensionIgnoreOctave.h"

BaseKeyScaleConverter::BaseKeyScaleConverter()
    : LinearScaleConverter(tDisplayRange(0, 11), NoteDimensionIgnoreOctave::get())
{
}

BaseKeyScaleConverter::~BaseKeyScaleConverter()
{
}
