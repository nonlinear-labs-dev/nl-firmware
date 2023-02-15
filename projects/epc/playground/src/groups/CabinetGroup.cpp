#include "CabinetGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

CabinetGroup::CabinetGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Cab", vg }, "Cabinet", "Cabinet", "Cabinet")
{
}