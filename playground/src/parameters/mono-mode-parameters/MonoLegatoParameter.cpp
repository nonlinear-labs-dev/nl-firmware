#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include "MonoLegatoParameter.h"

MonoLegatoParameter::MonoLegatoParameter(ParameterGroup *group, uint16_t id)
    : MonoParameter(group, id, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 1, 1)
{
}

ustring MonoLegatoParameter::getDisplayString() const
{
  if(getValue().getDisplayValue() != 0.0)
  {
    return "On";
  }
  else
  {
    return "Off";
  }
}

ustring MonoLegatoParameter::getLongName() const {
  return "Mono Legato";
}

ustring MonoLegatoParameter::getShortName() const {
  return getLongName();
}
