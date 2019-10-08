#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoPriorityParameter.h"

MonoPriorityParameter::MonoPriorityParameter(ParameterGroup *parent, Parameter::ID id)
    : MonoParameter(parent, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 2, 2)
{
}

ustring MonoPriorityParameter::getDisplayString() const
{
  auto value = getValue().getDisplayValue();
  if(value == 0.0)
  {
    return "Lowest";
  }
  else if(value == 0.5)
  {
    return "Last";
  }
  else
  {
    return "Highest";
  }
}

ustring MonoPriorityParameter::getLongName() const {
  return "Mono Priority";
}

ustring MonoPriorityParameter::getShortName() const {
  return getLongName();
}
