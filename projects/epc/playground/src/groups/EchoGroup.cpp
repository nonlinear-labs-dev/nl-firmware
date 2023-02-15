#include "EchoGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

EchoGroup::EchoGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Echo", vg }, "Echo", "Echo", "Echo")
{
}