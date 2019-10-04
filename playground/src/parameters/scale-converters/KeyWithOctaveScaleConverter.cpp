#include <parameters/scale-converters/dimension/NoteWithOctaveDimension.h>
#include "KeyWithOctaveScaleConverter.h"

KeyWithOctaveScaleConverter::KeyWithOctaveScaleConverter() : LinearScaleConverter(tTcdRange(36, 96), tDisplayRange(36, 96), NoteWithOctaveDimension::get()) {

}
