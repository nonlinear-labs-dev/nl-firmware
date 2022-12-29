#include "ReverbGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ReverbGroup::ReverbGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Reverb", vg }, "Reverb", "Reverb", "Reverb")
{
}

void ReverbGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Size, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Pre_Dly, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Color, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Chorus, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Send, getVoiceGroup() }));
}
