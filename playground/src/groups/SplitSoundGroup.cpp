#include <parameters/SplitPointParameter.h>
#include <parameters/mono-mode-parameters/MonoLegatoParameter.h>
#include <parameters/mono-mode-parameters/MonoModeEnableParameter.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include "SplitSoundGroup.h"

SplitSoundGroup::SplitSoundGroup(ParameterDualGroupSet* parent)
    : ParameterGroup(parent, "Split", "Split", "Split", "Split", VoiceGroup::Global)
{
  init();
}

void SplitSoundGroup::init()
{
  appendParameter(new SplitPointParameter(this, 18700));
}
