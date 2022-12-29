#include <groups/EnvelopeBGroup.h>
#include <parameters/ModulateableParameter.h>
#include "parameter_declarations.h"

EnvelopeBGroup::EnvelopeBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env B", vg }, "Envelope B", "Envelope B", "Envelope B")
{
}

void EnvelopeBGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Att, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Dec_1, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_BP, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Dec_2, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Sus, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Rel, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Gain, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Lvl_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Att_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Dec_1_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Dec_2_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Rel_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Lvl_KT, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_B_Time_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Att_Curve, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Elevate, getVoiceGroup() }));
}
