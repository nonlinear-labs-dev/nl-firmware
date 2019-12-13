#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoLegatoParameter.h"

MonoLegatoParameter::MonoLegatoParameter(ParameterGroup *group, ParameterId id)
    : Parameter(group, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 1, 1)
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

DFBLayout *MonoLegatoParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Select:
    default:
      return new MonoModeParameterLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Info:
      return new ParameterInfoLayout();
  }
}
