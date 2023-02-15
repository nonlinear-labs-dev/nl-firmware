#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include "MonoGroup.h"
#include <parameter_list.h>

MonoGroup::MonoGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mono", vg }, "Mono", "Mono", "Mono")
{
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
