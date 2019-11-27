#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>
#include <parameters/SplitPointParameter.h>
#include "GlobalParameterGroups.h"

GlobalParameterGroups::GlobalParameterGroups(ParameterDualGroupSet *parent)
    : ParameterGroup(parent, "Split", "Split", "Split", "Split", VoiceGroup::Global)
{
}

void GlobalParameterGroups::init()
{
  appendParameter(new SplitPointParameter(this, {356, VoiceGroup::Global}));
}
