#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoLegatoParameter.h"

MonoLegatoParameter::MonoLegatoParameter(ParameterGroup *group, ParameterId id)
    : MonoParameter(group, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 1, 1)
{
}

Glib::ustring MonoLegatoParameter::getDisplayString() const
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

Glib::ustring MonoLegatoParameter::getLongName() const
{
  return "Legato";
}

Glib::ustring MonoLegatoParameter::getShortName() const
{
  return "Legato";
}
