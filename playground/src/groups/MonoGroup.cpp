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
  appendParameter(new MonoModeEnableParameter(this, 12345));
  appendParameter(new MonoPriorityParameter(this, 12348));
  appendParameter(new MonoLegatoParameter(this, 12346));
  appendParameter(new MonoGlideTimeParameter(this, 12347));
}
