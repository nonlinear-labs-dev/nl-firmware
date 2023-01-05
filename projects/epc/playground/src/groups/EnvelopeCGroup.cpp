#include <groups/EnvelopeCGroup.h>
#include <parameters/ModulateableParameter.h>
#include <parameter_declarations.h>

EnvelopeCGroup::EnvelopeCGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env C", vg }, "Envelope C", "Envelope C", "Envelope C")
{
}

void EnvelopeCGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Att, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Dec_1, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Dec_1_Vel, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_BP, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Dec_2, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Dec_2_Vel, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Sus, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Loop, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Rel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Lvl_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Att_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Rel_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Lvl_KT, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Time_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Att_Curve, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::ParameterID::Env_C_Retr_H, getVoiceGroup() }));
}
