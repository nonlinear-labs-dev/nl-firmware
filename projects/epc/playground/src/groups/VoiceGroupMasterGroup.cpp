#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>
#include <parameters/scale-converters/Linear60KeyScaleConverter.h>
#include <parameters/scale-converters/dimension/PitchDimension.h>
#include <parameters/scale-converters/Linear60StScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar96StScaleConverterFine.h>
#include "VoiceGroupMasterGroup.h"
#include "parameter_declarations.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Part", vg }, "Part", "Part", "Part")
{
}

class FadeFrom : public VoiceGroupMasterUnmodulateableParameter
{
 public:
  FadeFrom(ParameterGroup* parent, const ParameterId& id, double initial)
      : VoiceGroupMasterUnmodulateableParameter(parent, id, ScaleConverter::get<Linear60KeyScaleConverter>())
  {
    getValue().setFactoryDefault(initial);
  }
};

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VoiceGroupMasterModulateableParameter(this, { C15::PID::Voice_Grp_Volume, getVoiceGroup() },
                                                            ScaleConverter::get<ParabolicGainDbScaleConverter>()));

  appendParameter(new VoiceGroupMasterModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Voice_Grp_Tune, getVoiceGroup() }, ScaleConverter::get<LinearBipolar48StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverterFine>()));

  appendParameter(new VoiceGroupMasterUnmodulateableParameter(this, { C15::PID::Voice_Grp_Mute, getVoiceGroup() },
                                                              ScaleConverter::get<OnOffScaleConverter>()));

  auto fadeFromInitial = getVoiceGroup() == VoiceGroup::I ? 1 : 0;

  appendParameter(new FadeFrom(this, { C15::PID::Voice_Grp_Fade_From, getVoiceGroup() }, fadeFromInitial));

  appendParameter(new VoiceGroupMasterUnmodulateableParameter(this, { C15::PID::Voice_Grp_Fade_Range, getVoiceGroup() },
                                                              ScaleConverter::get<Linear60StScaleConverter>()));
}
