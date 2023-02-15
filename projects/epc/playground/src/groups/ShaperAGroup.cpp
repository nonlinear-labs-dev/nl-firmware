#include "ShaperAGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

ShaperAGroup::ShaperAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Sh A", vg }, "Shaper A", "Shaper A", "Shaper A")
{
}