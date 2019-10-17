#include <parameters/scale-converters/dimension/NoteWithOctaveDimension.h>
#include "KeyWithOctaveScaleConverter.h"

KeyWithOctaveScaleConverter::KeyWithOctaveScaleConverter()
    : LinearScaleConverter(tTcdRange(0, 60), tDisplayRange(0, 60), NoteWithOctaveDimension::get())
{
}
