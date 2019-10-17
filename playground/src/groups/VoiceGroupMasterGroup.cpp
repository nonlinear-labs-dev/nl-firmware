#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/SpecialGlobalParameter.h>
#include "VoiceGroupMasterGroup.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup)
    : ParameterGroup(parent, "VGM", "VG Master", "Voice Group Master", "Voice Group Master", voicegroup)
{
}

void VoiceGroupMasterGroup::init()
{
  appendParameter(new SpecialPolyParameter(this, 10002, ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.5,
                                             100, 1000, "Level", "Output Level", getVoiceGroup()));
  appendParameter(new SpecialPolyParameter(this, 10003, ScaleConverter::get<LinearBipolar48StScaleConverter>(), 0, 48,
                                             4800, "Tune", "Voice Group Tune", getVoiceGroup()));
}