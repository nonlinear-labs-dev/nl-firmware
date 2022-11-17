#include "OutputMixerGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

OutputMixerGroup::OutputMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mixer", vg }, "Output Mixer", "Output Mixer", "Output Mixer")
{
}

OutputMixerGroup::~OutputMixerGroup()
{
}

void OutputMixerGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_A_Lvl, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_A_Pan, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_B_Lvl, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_B_Pan, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Comb_Lvl, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Comb_Pan, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_SVF_Lvl, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_SVF_Pan, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Drive, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Fold, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Asym, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Lvl, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Out_Mix_Key_Pan, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_To_FX, getVoiceGroup() }));
}
