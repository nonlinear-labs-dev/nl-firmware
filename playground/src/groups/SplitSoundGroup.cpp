#include <parameters/SplitPointParameter.h>
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
