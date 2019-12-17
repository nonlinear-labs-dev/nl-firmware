#include <parameters/scale-converters/EditSmoothingTimeMSScaleConverter.h>
#include "MonoGlideTimeParameter.h"

MonoGlideTimeParameter::MonoGlideTimeParameter(ParameterGroup *group, const ParameterId &id)
    : ModulateableMonoParameter(group, id, ScaleConverter::get<EditSmoothingTimeMSScaleConverter>(), 0, 100, 1000)
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
