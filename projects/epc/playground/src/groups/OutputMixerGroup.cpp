#include "OutputMixerGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

OutputMixerGroup::OutputMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mixer", vg }, "Output Mixer", "Output Mixer", "Output Mixer")
{
}