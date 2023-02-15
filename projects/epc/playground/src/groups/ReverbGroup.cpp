#include "ReverbGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ReverbGroup::ReverbGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Reverb", vg }, "Reverb", "Reverb", "Reverb")
{
}