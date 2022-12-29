#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h>
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
      : VoiceGroupMasterUnmodulateableParameter(parent, id)
  {
    getValue().setFactoryDefault(initial);
  }
};

void VoiceGroupMasterGroup::init()
{
  appendParameter(new VoiceGroupMasterModulateableParameter(this, { C15::PID::Voice_Grp_Volume, getVoiceGroup() }));
  appendParameter(new VoiceGroupMasterModulateableParameter(this, { C15::PID::Voice_Grp_Tune, getVoiceGroup() }));
  appendParameter(new VoiceGroupMasterUnmodulateableParameter(this, { C15::PID::Voice_Grp_Mute, getVoiceGroup() }));

  auto fadeFromInitial = getVoiceGroup() == VoiceGroup::I ? 1 : 0;

  appendParameter(new FadeFrom(this, { C15::PID::Voice_Grp_Fade_From, getVoiceGroup() }, fadeFromInitial));
  appendParameter(
      new VoiceGroupMasterUnmodulateableParameter(this, { C15::PID::Voice_Grp_Fade_Range, getVoiceGroup() }));
}
