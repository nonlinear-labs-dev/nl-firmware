#include <parameters/mono-mode-parameters/MonoModeEnableParameter.h>
#include <parameters/mono-mode-parameters/MonoLegatoParameter.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/mono-mode-parameters/MonoPriorityParameter.h>
#include "MonoGroup.h"

MonoGroup::MonoGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mono", vg }, "Mono", "Mono", "Mono")
{
}

void MonoGroup::init()
{
  appendParameter(new MonoModeEnableParameter(this, { 364, getVoiceGroup() }));
  appendParameter(new MonoPriorityParameter(this, { 365, getVoiceGroup() }));
  appendParameter(new MonoLegatoParameter(this, { 366, getVoiceGroup() }));
  appendParameter(new MonoGlideTimeParameter(this, { 367, getVoiceGroup() }));
}
