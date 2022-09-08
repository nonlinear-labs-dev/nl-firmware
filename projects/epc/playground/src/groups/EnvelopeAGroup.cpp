#include <groups/EnvelopeAGroup.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include "parameter_declarations.h"

EnvelopeAGroup::EnvelopeAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env A", vg }, "Envelope A", "Envelope A", "Envelope A")
{
}

EnvelopeAGroup::~EnvelopeAGroup()
{
}

void EnvelopeAGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Att, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Dec_1, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_BP, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Dec_2, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Sus, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Rel, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Gain, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Att_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_1_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_2_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Rel_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_KT, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_A_Time_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Att_Curve, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Elevate, getVoiceGroup() }));
}
