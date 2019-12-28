#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include "VoiceGroupMasterGroup.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Part", vg }, "Part", "Part", "Part")
{
}

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VoiceGroupMasterParameter(this, { 358, getVoiceGroup() },
                                                ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.5, 100, 1000,
                                                "Volume", "Part Volume", getVoiceGroup()));

  appendParameter(new VoiceGroupMasterParameter(this, { 360, getVoiceGroup() },
                                                ScaleConverter::get<LinearBipolar48StScaleConverter>(), 0, 48, 4800,
                                                "Tune", "Part Tune", getVoiceGroup()));

  appendParameter(new VoiceGroupMasterParameter(this, { 362, getVoiceGroup() },
                                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000,
                                                "To FX", "To FX", getVoiceGroup()));
}
