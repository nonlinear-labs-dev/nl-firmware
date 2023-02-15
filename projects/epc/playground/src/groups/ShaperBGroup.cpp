#include "ShaperBGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ShaperBGroup::ShaperBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Sh B", vg }, "Shaper B", "Shaper B", "Shaper B")
{
}