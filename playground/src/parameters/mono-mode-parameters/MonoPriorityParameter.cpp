#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoPriorityParameter.h"

MonoPriorityParameter::MonoPriorityParameter(ParameterGroup *parent, const ParameterId &id)
    : UnmodulateableMonoParameter(parent, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 2, 2)
{
}

Glib::ustring MonoPriorityParameter::getDisplayString() const
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

Glib::ustring MonoPriorityParameter::getLongName() const
{
  return "Priority";
}

Glib::ustring MonoPriorityParameter::getShortName() const
{
  return "Prio";
}