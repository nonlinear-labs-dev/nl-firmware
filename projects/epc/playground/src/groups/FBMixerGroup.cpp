#include "FBMixerGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

FBMixerGroup::FBMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "FB", vg }, "FB Mixer", "Feedback Mixer", "Feedback Mixer")
{
}

FBMixerGroup::~FBMixerGroup()
{
}

void FBMixerGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Osc, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Osc_Src, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Comb, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Comb_Src, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_SVF, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_SVF_Src, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_FX, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_FX_Src, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Rvb, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Drive, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Fold, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Asym, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::FB_Mix_Lvl_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Lvl, getVoiceGroup() }));
}
