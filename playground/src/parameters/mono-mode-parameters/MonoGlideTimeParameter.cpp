#include <parameters/scale-converters/EditSmoothingTimeMSScaleConverter.h>
#include "MonoGlideTimeParameter.h"

MonoGlideTimeParameter::MonoGlideTimeParameter(ParameterGroup *group, ParameterId id)
    : ModulateableParameter(group, id, ScaleConverter::get<EditSmoothingTimeMSScaleConverter>(), 0, 100, 1000)
{
}

Glib::ustring MonoGlideTimeParameter::getLongName() const
{
  return "Glide Time";
}

Glib::ustring MonoGlideTimeParameter::getShortName() const
{
  return "Glide";
}

DFBLayout *MonoGlideTimeParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Select:
    default:
      return new MonoModeModulateableParameterLayout();
    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();
    case UIMode::Info:
      return new ParameterInfoLayout();
  }
}
