#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include "MonoGroup.h"
#include <parameter_list.h>

MonoGroup::MonoGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mono", vg }, "Mono", "Mono", "Mono")
{
}

void MonoGroup::init()
{
  appendParameter(new UnmodulateableMonoParameter(this, { C15::PID::Mono_Grp_Enable, getVoiceGroup() }));
  appendParameter(new UnmodulateableMonoParameter(this, { C15::PID::Mono_Grp_Prio, getVoiceGroup() }));
  appendParameter(new UnmodulateableMonoParameter(this, { C15::PID::Mono_Grp_Legato, getVoiceGroup() }));
  appendParameter(new MonoGlideTimeParameter(this, { C15::PID::Mono_Grp_Glide, getVoiceGroup() }));
}

bool MonoGroup::isMonoParameter(const ParameterId& id)
{
  using namespace C15::PID;
  auto number = id.getNumber();
  return number == Mono_Grp_Enable || number == Mono_Grp_Prio || number == Mono_Grp_Legato || number == Mono_Grp_Glide;
}

bool MonoGroup::isMonoParameter(const Parameter* parameter)
{
  if(parameter)
    return isMonoParameter(parameter->getID());
  return false;
}
