#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h>
#include "VoiceGroupMasterGroup.h"
#include "parameter_declarations.h"

VoiceGroupMasterGroup::VoiceGroupMasterGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Part", vg }, "Part", "Part", "Part")
{
}

void VoiceGroupMasterGroup::init()
{
  ParameterGroup::init();
  auto fadeFromInitial = getVoiceGroup() == VoiceGroup::I ? 1 : 0;
  auto fadeFrom = findParameterByID({ C15::PID::Voice_Grp_Fade_From, getVoiceGroup() });
  fadeFrom->getValue().setFactoryDefault(fadeFromInitial);
}

bool VoiceGroupMasterGroup::isVoiceGroupMasterParameter(int id)
{
  return id == C15::PID::Voice_Grp_Volume || id == C15::PID::Voice_Grp_Tune || id == C15::PID::Voice_Grp_Mute
      || id == C15::PID::Voice_Grp_Fade_From || id == C15::PID::Voice_Grp_Fade_Range;
}
