#include <groups/FlangerGroup.h>
#include <parameters/ModulateableParameter.h>
#include "parameter_declarations.h"

FlangerGroup::FlangerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Flang", vg }, "Flanger", "Flanger", "Flanger")
{
}
