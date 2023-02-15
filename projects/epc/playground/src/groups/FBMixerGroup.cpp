#include "FBMixerGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

FBMixerGroup::FBMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "FB", vg }, "FB Mixer", "Feedback Mixer", "Feedback Mixer")
{
}