#include <parameters/scale-converters/EditSmoothingTimeMSScaleConverter.h>
#include "MonoGlideTimeParameter.h"

MonoGlideTimeParameter::MonoGlideTimeParameter(ParameterGroup *group, Parameter::ID id)
    : MonoParameter(group, id, ScaleConverter::get<EditSmoothingTimeMSScaleConverter>(), 0, 100, 1000)
{
}

ustring MonoGlideTimeParameter::getLongName() const {
  return "Glide Time";
}

ustring MonoGlideTimeParameter::getShortName() const {
  return "Glide";
}
