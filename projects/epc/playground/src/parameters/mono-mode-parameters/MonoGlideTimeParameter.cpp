#include "MonoGlideTimeParameter.h"
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>

MonoGlideTimeParameter::MonoGlideTimeParameter(ParameterGroup *group, const ParameterId &id)
    : ModulateableMonoParameter(group, id)
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
