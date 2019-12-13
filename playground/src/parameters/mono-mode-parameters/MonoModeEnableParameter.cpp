#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "MonoModeEnableParameter.h"

MonoModeEnableParameter::MonoModeEnableParameter(ParameterGroup *group, ParameterId id)
    : Parameter(group, id, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 1, 1)
{
}

Glib::ustring MonoModeEnableParameter::getDisplayString() const
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

Glib::ustring MonoModeEnableParameter::getLongName() const
{
  return "Mono Enable";
}

Glib::ustring MonoModeEnableParameter::getShortName() const
{
  return "Enable";
}

DFBLayout *MonoModeEnableParameter::createLayout(FocusAndMode focusAndMode) const {
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
