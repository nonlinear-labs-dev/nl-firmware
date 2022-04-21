#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/OnOffDimension.h>
#include <parameters/scale-converters/MonoPriorityScaleConverter.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <parameters/scale-converters/LegatoScaleConverter.h>
#include "MonoGroup.h"

MonoGroup::MonoGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mono", vg }, "Mono", "Mono", "Mono")
{
}

void MonoGroup::init()
{
  appendParameter(
      new UnmodulateableMonoParameter(this, { 364, getVoiceGroup() }, ScaleConverter::get<OnOffScaleConverter>()));
  appendParameter(new UnmodulateableMonoParameter(this, { 365, getVoiceGroup() },
                                                  ScaleConverter::get<MonoPriorityScaleConverter>()));
  appendParameter(
      new UnmodulateableMonoParameter(this, { 366, getVoiceGroup() }, ScaleConverter::get<LegatoScaleConverter>()));
  appendParameter(new MonoGlideTimeParameter(this, { 367, getVoiceGroup() }));
}

bool MonoGroup::isMonoParameter(const ParameterId& id)
{
  auto number = id.getNumber();
  return number == 364 || number == 365 || number == 366 || number == 367;
}

bool MonoGroup::isMonoParameter(const Parameter* parameter)
{
  if(parameter)
    return isMonoParameter(parameter->getID());
  return false;
}
