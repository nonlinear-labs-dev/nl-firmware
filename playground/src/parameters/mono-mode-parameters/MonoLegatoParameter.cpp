#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoLegatoParameter.h"

MonoLegatoParameter::MonoLegatoParameter(ParameterGroup *group, ParameterId id)
    : MonoParameter(group, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 1, 1)
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

ustring MonoLegatoParameter::getLongName() const
{
  return "Legato";
}

ustring MonoLegatoParameter::getShortName() const
{
  return "Legato";
}