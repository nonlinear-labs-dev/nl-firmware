#include "OscillatorAGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

OscillatorAGroup::OscillatorAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc A", vg }, "Oscillator A", "Oscillator A", "Oscillator A")
{
}

OscillatorAGroup::~OscillatorAGroup()
{
}

void OscillatorAGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Pitch, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Osc_A_Pitch_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Pitch_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Fluct, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Fluct_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_Self, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_Self_Env_A, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_Self_Shp, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_B_Env_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_B_Shp, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_FB, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_FB_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Phase, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Chirp, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Osc_A_Reset, getVoiceGroup() }));
}
