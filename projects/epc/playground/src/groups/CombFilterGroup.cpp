#include "CombFilterGroup.h"

#include "parameters/ModulateableParameter.h"
#include "parameters/Parameter.h"
#include "parameter_declarations.h"

CombFilterGroup::CombFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Comb", vg }, "Comb Filter", "Comb Filter", "Comb Filter")
{
}

CombFilterGroup::~CombFilterGroup()
{
}

void CombFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_In_A_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Pitch, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Pitch_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Pitch_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay_Gate, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_Tune, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_Res, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_LP_Tune, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_LP_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_LP_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_PM, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_PM_A_B, getVoiceGroup() }));
}
