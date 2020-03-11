#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/LinearBipolar96StScaleConverter.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>
#include <parameters/scale-converters/Linear60KeyScaleConverter.h>
#include <parameters/scale-converters/dimension/PitchDimension.h>
#include "VoiceGroupMasterGroup.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Part", vg }, "Part", "Part", "Part")
{
}

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VoiceGroupMasterModulateableParameter(
      this, { 358, getVoiceGroup() }, ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new VoiceGroupMasterModulateableParameterWithUnusualModUnit(
      this, { 360, getVoiceGroup() }, ScaleConverter::get<LinearBipolar48StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverter>(), 0, 48, 4800));

  appendParameter(new VoiceGroupMasterUnmodulateableParameter(this, { 395, getVoiceGroup() },
                                                              ScaleConverter::get<OnOffScaleConverter>(), 0.0, 1, 1));

  appendParameter(new VoiceGroupMasterUnmodulateableParameter(
      this, { 396, getVoiceGroup() }, ScaleConverter::get<Linear60KeyScaleConverter>(), 0.5, 60, 60));

  appendParameter(new VoiceGroupMasterUnmodulateableParameter(
      this, { 397, getVoiceGroup() }, ScaleConverter::get<LinearCountScaleConverter<60, PitchDimension>>(), 0.0, 60,
      60));
}
