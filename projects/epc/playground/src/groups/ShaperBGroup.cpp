#include "ShaperBGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ShaperBGroup::ShaperBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Sh B", vg }, "Shaper B", "Shaper B", "Shaper B")
{
}

ShaperBGroup::~ShaperBGroup()
{
}

void ShaperBGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Drive, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Drive_Env_B, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Fold, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Asym, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_FB_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_FB_Env_C, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_B_Ring_Mod, getVoiceGroup() }));
}
