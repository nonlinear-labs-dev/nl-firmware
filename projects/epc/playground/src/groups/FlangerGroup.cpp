#include <groups/FlangerGroup.h>
#include <parameters/ModulateableParameter.h>
#include "parameter_declarations.h"

FlangerGroup::FlangerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Flang", vg }, "Flanger", "Flanger", "Flanger")
{
}

void FlangerGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Time_Mod, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Phase, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Rate, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Time, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Stereo, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Feedback, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Cross_FB, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Hi_Cut, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Envelope, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_AP_Mod, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_AP_Tune, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Tremolo, getVoiceGroup() }));
}
