#include <parameters/mono-mode-parameters/MonoModeEnableParameter.h>
#include <parameters/mono-mode-parameters/MonoLegatoParameter.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/mono-mode-parameters/MonoPriorityParameter.h>
#include "MonoGroup.h"

MonoGroup::MonoGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup)
    : ParameterGroup(parent, "Mono", "Mono", "Mono", "Mono", voicegroup)
{
}

void MonoGroup::init()
{
#warning "Really!?"
  appendParameter(new MonoModeEnableParameter(this, { 12345, getVoiceGroup() }));
  appendParameter(new MonoLegatoParameter(this, { 12346, getVoiceGroup() }));
  appendParameter(new MonoGlideTimeParameter(this, { 12347, getVoiceGroup() }));
  appendParameter(new MonoPriorityParameter(this, { 12348, getVoiceGroup() }));
}
