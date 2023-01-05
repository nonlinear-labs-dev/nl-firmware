#include "ShaperAGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ShaperAGroup::ShaperAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Sh A", vg }, "Shaper A", "Shaper A", "Shaper A")
{
}

void ShaperAGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Drive, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Drive_Env_A, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Fold, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Asym, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_FB_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_FB_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Ring_Mod, getVoiceGroup() }));
}
