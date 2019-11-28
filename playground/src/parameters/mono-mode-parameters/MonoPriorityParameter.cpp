#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoPriorityParameter.h"

MonoPriorityParameter::MonoPriorityParameter(ParameterGroup *parent, ParameterId id)
    : MonoParameter(parent, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 2, 2)
{
}

ustring MonoPriorityParameter::getDisplayString() const
{
  auto value = getValue().getDisplayValue();
  if(value == 0)
  {
    return "Lowest";
  }
  else if(value == 50)
  {
    return "Latest";
  }
  else
  {
    return "Highest";
  }
}

ustring MonoPriorityParameter::getLongName() const
{
  return "Priority";
}

ustring MonoPriorityParameter::getShortName() const
{
  return "Prio";
}
