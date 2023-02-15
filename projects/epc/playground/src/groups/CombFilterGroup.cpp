#include "CombFilterGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

CombFilterGroup::CombFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Comb", vg }, "Comb Filter", "Comb Filter", "Comb Filter")
{
}