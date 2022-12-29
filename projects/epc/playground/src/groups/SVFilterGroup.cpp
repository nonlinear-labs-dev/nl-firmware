#include "SVFilterGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

SVFilterGroup::SVFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "SVF", vg }, "SV Filter", "State Variable Filter", "State Variable Filter")
{
}

void SVFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_In_A_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Comb_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Cut, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Cut_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Cut_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Res, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Res_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Res_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Spread, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_LBH, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Par, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_FM, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_FM_A_B, getVoiceGroup() }));
}
