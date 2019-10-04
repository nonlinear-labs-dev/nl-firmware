#include <testing/TestDriver.h>
#include <parameters/scale-converters/KeyWithOctaveScaleConverter.h>
#include "SplitPointParameter.h"

SplitPointParameter::SplitPointParameter(ParameterGroup *group, uint16_t id, tControlPositionValue def)
    : Parameter(group, id, ScaleConverter::get<KeyWithOctaveScaleConverter>(), def, 1, 1)
{
}

std::string SplitPointParameter::getDisplayValue(VoiceGroup vg) const
{
  auto converter = ScaleConverter::get<KeyWithOctaveScaleConverter>();
  if(vg == VoiceGroup::I)
    return converter->getDimension().stringize(static_cast<int>(getValue().getRawValue()));
  else if(vg == VoiceGroup::II)
    return converter->getDimension().stringize(static_cast<int>(getValue().getRawValue()) - 1);

  return "";
}
