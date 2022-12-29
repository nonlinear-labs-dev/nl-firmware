#include "GapFilterGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

GapFilterGroup::GapFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Gap Filt", vg }, "Gap Filter", "Gap Filter", "Gap Filter")
{
}

void GapFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Center, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Stereo, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Gap, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Res, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Bal, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Mix, getVoiceGroup() }));
}
