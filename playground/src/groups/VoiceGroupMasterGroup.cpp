#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>
#include "VoiceGroupMasterGroup.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup)
    : ParameterGroup(parent, "PART", "Part", "Part Master", "Part Master", voicegroup)
{
}

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VoiceGroupMasterParameter(this, 10002, ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.5, 100,
                                                1000, "Level", "Part Level", getVoiceGroup()));
  appendParameter(new VoiceGroupMasterParameter(this, 10003, ScaleConverter::get<LinearBipolar48StScaleConverter>(), 0, 48,
                                                4800, "Tune", "Part Tune", getVoiceGroup()));
}