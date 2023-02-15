#include "SVFilterGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

SVFilterGroup::SVFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "SVF", vg }, "SV Filter", "State Variable Filter", "State Variable Filter")
{
}