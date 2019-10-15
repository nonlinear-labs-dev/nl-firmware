#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VGMasterParameter.h>
#include "VoiceGroupMasterGroup.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup)
    : ParameterGroup(parent, "VGM", "VGM", "VGM", "VGM", voicegroup)
{
}

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VGMasterParameter(this, 11247, ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.5, 100, 1000));
  appendParameter(new VGMasterParameter(this, 11248, ScaleConverter::get<LinearBipolar48StScaleConverter>(), 0, 48, 4800));
}
