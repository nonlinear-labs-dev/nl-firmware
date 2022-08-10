#include "Linear12CountScaleConverter.h"
#include "dimension/VoicesDimension.h"

Linear12CountScaleConverter::Linear12CountScaleConverter()
    : LinearScaleConverter(tDisplayRange(1, 12), VoicesDimension::get())
{
}

Linear12CountScaleConverter::~Linear12CountScaleConverter()
{
}
