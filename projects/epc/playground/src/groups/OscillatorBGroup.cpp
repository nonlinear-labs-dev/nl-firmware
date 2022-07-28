#include "OscillatorBGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

OscillatorBGroup::OscillatorBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc B", vg }, "Oscillator B", "Oscillator B", "Oscillator B")
{
}

OscillatorBGroup::~OscillatorBGroup()
{
}

void OscillatorBGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Pitch, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Osc_B_Pitch_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Pitch_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Fluct, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Fluct_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_Self, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_Self_Env_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_Self_Shp, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_A, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_A_Env_A, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_A_Shp, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_FB, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_FB_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Phase, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Chirp, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Osc_B_Reset, getVoiceGroup() }));
}
