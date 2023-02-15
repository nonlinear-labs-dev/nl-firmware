#include "GapFilterGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

GapFilterGroup::GapFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Gap Filt", vg }, "Gap Filter", "Gap Filter", "Gap Filter")
{
}