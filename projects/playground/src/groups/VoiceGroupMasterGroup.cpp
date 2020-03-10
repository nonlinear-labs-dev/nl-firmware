#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/LinearBipolar96StScaleConverter.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>
#include <parameters/scale-converters/SplitPointScaleConverter.h>
#include <parameters/scale-converters/dimension/UnitlessDimension.h>
#include <parameters/scale-converters/Linear60KeyScaleConverter.h>
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

  appendParameter(new VoiceGroupMasterParameterWithUnusualModUnit(
      this, { 360, getVoiceGroup() }, ScaleConverter::get<LinearBipolar48StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverter>(), 0, 48, 4800, "Part Tune", "Tune"));

  appendParameter(new Parameter(this, { 395, getVoiceGroup() }, ScaleConverter::get<OnOffScaleConverter>(), 0.0, 1, 1));

  appendParameter(
      new Parameter(this, { 396, getVoiceGroup() }, ScaleConverter::get<Linear60KeyScaleConverter>(), 0.5, 60, 60));

  appendParameter(new Parameter(this, { 397, getVoiceGroup() },
                                ScaleConverter::get<LinearCountScaleConverter<60, UnitlessDimension>>(), 0.0, 59, 59));
}
