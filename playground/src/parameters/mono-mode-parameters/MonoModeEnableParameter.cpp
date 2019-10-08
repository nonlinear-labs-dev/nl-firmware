#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoModeEnableParameter.h"

MonoModeEnableParameter::MonoModeEnableParameter(ParameterGroup *group, uint16_t id)
    : MonoParameter(group, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 1, 1)
{
}

ustring MonoModeEnableParameter::getDisplayString() const
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

ustring MonoModeEnableParameter::getLongName() const {
  return "Mono Enable";
}

ustring MonoModeEnableParameter::getShortName() const {
  return getLongName();
}
